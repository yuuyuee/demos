// Copyright 2022 The Oak Authors.

#include "oak/addons/module.h"

#include <assert.h>
#include <dlfcn.h>

#include <utility>
#include <algorithm>

#include "oak/common/stringpiece.h"
#include "oak/logging/logging.h"
#include "oak/common/fs.h"

namespace oak {

// ModuleBase

ModuleBase::ModuleBase() = default;
ModuleBase::~ModuleBase() = default;

// Load cpp module with entry name from the path, if path has empty,
// load from main program.
// int OpenModule(int id, StringPiece entry, StringPiece path) {
//   assert(!entry.empty() && "invalid module entry");
//   int ret;
//   if (path.empty() || path.ends_with(".so")) {
//     ret = OpenCppModule(id, entry, path);
//   } else if (StringPiece(path).ends_with(".py")) {
//     ret = OpenCppModule(id, entry, path);
//   } else {
//     OAK_ERROR("Open module %*s: Unidentified module\n",
//               path.size(), path.data());
//     ret = -1;
//   }

//   if (ret == 0)
//     ownership_ = true;

//   return ret;
// }

// Load cpp module with entry name from the path, if path has empty,
// load from main program.
void* OpenCppModule(const std::string& entry, const std::string& path) {
  const char* rpath = path.empty() ? nullptr : path.c_str();
  const char* err = dlerror();
  void* handle = dlopen(rpath, RTLD_NOW | RTLD_GLOBAL);
  if (!handle) {
    OAK_ERROR("Open module %s: dlopen failed: %s\n",
              entry.c_str(), dlerror());
    return nullptr;
  }
  return handle;

  // dl_handle_ = handle;

  // const char* err = dlerror();
  // void* cpp_handle = dlsym(handle, m_entry.c_str());
  // if (!handle) {
  //   CloseCppModule();
  //   OAK_ERROR("Open module %s: Not found entry: %s\n",
  //             m_entry.c_str(), dlerror());
  //   return -1;
  // }

  // m_cpp_handle = static_cast<struct oak_cpp_handle*>(cpp_handle);
  // m_version = m_cpp_handle->version;
  // if ((m_version >> 16) < OAK_VERSION_MAJOR) {
  //   CloseCppModule();
  //   OAK_ERROR("Open module %s: Incompatible version\n", m_entry.c_str());
  //   return -1;
  // }
  // m_type = static_cast<ModuleType>(m_cpp_handle->flags);
  // m_lang_type = LANG_TYPE_C_CPP;
}

// Close module to free handler.
void CloseCppModule(const std::string& entry, void* handle) {
  const char* err = dlerror();
  int ret = dlclose(handle);
  if (ret != 0) {
    OAK_ERROR("Close module %s: dlclose() failed: %s\n",
              entry.c_str(), dlerror());
  }
}

// Loading python module with entry name from the path.
void* OpenPyModule(const std::string& entry, const std::string& path) {
  // TODO(YUYUE): Not implemented
  OAK_ERROR("Open module %s: Not implemented\n", entry.c_str());
  return nullptr;
}

// Close module to free handler.
void ClosePyModule(const std::string& entry, void*) {
  // TODO(YUYUE): Not implemented
  OAK_ERROR("Open module %s: Not implemented\n", entry.c_str());
}

void DumpModule(const ModuleBase& base) {
  OAK_INFO(
      "Module ID: %d\n"
      "Module Version: %d\n"
      "Module Entry: %s\n"
      "Module Path: %s\n"
      "Module Type: %s\n"
      "Module Language Type: %s\n"
      "Module loaded successfully.\n",
      base.id(),
      base.version(),
      base.name().c_str(),
      base.path().empty() ? "builtin" : base.path().c_str(),
      ModuleTypeName(base.type()),
      LangTypeName(base.lang_type()));
}

}  // namespace oak
