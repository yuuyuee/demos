// Copyright 2022 The Oak Authors.

#include "oak/addons/module.h"

#include <assert.h>
#include <dlfcn.h>
#include <string.h>

#include "oak/common/throw_delegate.h"
#include "oak/common/stringpiece.h"
#include "oak/logging/logging.h"
#include "oak/common/fs.h"

using std::string;

namespace oak {
static_assert(offsetof(oak_common_header, flags) ==
              offsetof(oak_source_module, flags),
              "logic error");
static_assert(offsetof(oak_common_header, flags) ==
              offsetof(oak_parser_module, flags),
              "logic error");
static_assert(offsetof(oak_common_header, flags) ==
              offsetof(oak_sink_module, flags),
              "logic error");

void InitModule(Module* module) {
  module->id = -1;
  module->name[0] = '\0';
  module->path[0] = '\0';
  module->type = MODULE_TYPE_UNKNOWN;
  module->lang_type = LANG_TYPE_UNKNOWN;
  module->header = nullptr;
  module->dl_handler = nullptr;
}

namespace {

int OpenCppModule(Module* module, int id,
                  const string& symbol,
                  const string& path) {
  const char* err = dlerror();
  const char* rpath = path.empty() ? nullptr : path.c_str();
  void* handler = dlopen(rpath, RTLD_NOW | RTLD_GLOBAL);
  if (!handler) {
    OAK_ERROR("dlopen(%s) failed: %s\n", rpath, dlerror());
    return -1;
  }

  const struct oak_common_header* header =
      reinterpret_cast<const struct oak_common_header*>(
          dlsym(handler, symbol.c_str()));
  if (!header) {
    OAK_ERROR("dlsym(%s) failed: %s\n", symbol.c_str(), dlerror());
    return -1;
  }

  module->id = id;
  memcpy(module->name, symbol.c_str(),
      symbol.size() < OAK_NAME_MAX ? symbol.size() : OAK_NAME_MAX);
  memcpy(module->path, path.c_str(),
      path.size() < PATH_MAX ? path.size() : PATH_MAX);
  module->type = static_cast<ModuleType>(header->flags);
  module->lang_type = LANG_TYPE_C_CPP;
  module->dl_handler = handler;
  return 0;
}

int OpenCppModule(Module*, int, const std::string&, const std::string&) {
  return -1;
}

}  // anonymous namespace

int OpenModule(Module* module, int id,
               const string& symbol,
               const string& path) {
  assert(!symbol.empty() && "invalid symbol");

  int ret = -1;
  if (path.empty() || StringPiece(path).ends_with(".so")) {
    ret = OpenCppModule(module, id, symbol, path.c_str());
  } else if (StringPiece(path).ends_with(".py")) {
    ret = OpenCppModule(module, id, symbol, path.c_str());
  }
  return ret;
}

void CloseModule(Module* module) {
  const char* err = dlerror();
  int ret = dlclose(module->dl_handler);
  if (ret != 0)
    OAK_ERROR("dlclose() failed: %s\n", dlerror());
}

}  // namespace oak
