// Copyright 2022 The Oak Authors.

#include "oak/addons/sink_handle.h"

#include <assert.h>
#include <dlfcn.h>
#include "oak/addons/metadata_internal.h"
#include "oak/logging/logging.h"

using std::string;
using std::unique_ptr;

namespace oak {

// SinkHandle

SinkHandle::SinkHandle(size_t id, const string& name, const string& path)
    : ModuleBase(id, name, path) {}

SinkHandle::~SinkHandle() {}

// CppSinkHandle

// Cpp implemented sink module.

class CppSinkHandle: public SinkHandle {
 public:
  explicit CppSinkHandle(size_t id,
                         const string& name,
                         const string& path,
                         void* dl_handle,
                         struct oak_sink_module* handle,
                         void* context);

  virtual ~CppSinkHandle();

  CppSinkHandle(CppSinkHandle const&) = delete;
  CppSinkHandle& operator=(CppSinkHandle const&) = delete;

  // Write the metadata to module specified the sink.
  virtual int Write(const struct oak_metadata* metadata) {
    return handle_->write(context_, metadata);
  }

 private:
  void* dl_handle_;
  struct oak_sink_module* handle_;
  void* context_;
};

CppSinkHandle::CppSinkHandle(size_t id,
                             const string& name,
                             const string& path,
                             void* dl_handle,
                             struct oak_sink_module* handle,
                             void* context)
    : SinkHandle(id, name, path),
      dl_handle_(dl_handle), handle_(handle), context_(context) {
  assert(!name.empty() && "Invalid module name");
  assert(dl_handle && "Invalid dl_handle");
  assert(handle && "Invalid handle");
  version_ = handle_->version;
  type_ = MODULE_TYPE_SINK;
  lang_type_ = LANG_TYPE_C_CPP;
}

CppSinkHandle::~CppSinkHandle() {
  handle_->close(context_);
  dlerror();
  if (dlclose(dl_handle_) != 0) {
    OAK_WARNING("Close module %s: dlclose failed: %s\n",
                name_.c_str(), dlerror());
  }
}

int CppEventHandleFactory(const ModuleConfig& module_config,
                          unique_ptr<SinkHandle>* module_handle) {
  assert(!module_config.name.empty() && "Invalid module name");
  const char* path = module_config.path.empty() ?
      nullptr : module_config.path.c_str();
  dlerror();
  void* dl_handle = dlopen(path, RTLD_NOW | RTLD_GLOBAL);
  if (!dl_handle) {
    OAK_ERROR("Open module %s: dlopen failed: %s\n",
              module_config.name.c_str(), dlerror());
    return -1;
  }

  dlerror();
  void* symbol = dlsym(dl_handle, module_config.name.c_str());
  if (!symbol) {
    OAK_ERROR("Open module %s: dlsym failed: %s\n",
              module_config.name.c_str(), dlerror());
    dlclose(dl_handle);
    return -1;
  }

  struct oak_sink_module* handle =
      reinterpret_cast<struct oak_sink_module*>(symbol);

  if (handle->flags != MODULE_TYPE_SINK) {
    OAK_ERROR("Open module %s: Unidentify module flags: %d\n",
              module_config.name.c_str(), handle->flags);
    dlclose(dl_handle);
    return -1;
  }

  if ((handle->version >> 16) != OAK_VERSION_MAJOR) {
    OAK_ERROR("Open module %s: Module version is too old: %d\n",
              module_config.name.c_str(), handle->version);
    dlclose(dl_handle);
    return -1;
  }

  struct oak_dict dict;
  oak_dict_init2(&dict);
  for (auto const& it : module_config.config) {
    if (dict.size >= OAK_DICT_DFL_CAP)
      break;
    if (it.first.empty() || it.second.empty())
      continue;
    oak_dict_ref(&dict,
        it.first.c_str(), it.first.size(),
        it.second.c_str(), it.second.size());
  }

  void* context = nullptr;
  int ret = handle->init(&context, &dict);
  if (ret != 0) {
    OAK_ERROR("Open module %s: Module initialization failed\n",
              module_config.name.c_str());
    dlclose(dl_handle);
    return -1;
  }

  CppSinkHandle* object =
      new CppSinkHandle(module_config.id,
                        module_config.name,
                        module_config.path,
                        dl_handle, handle, context);
  object->Dump();
  module_handle->reset(object);
  return 0;
}

// Create a sink handle, return 0 on success, -1 if any error
// occursed.
int SinkHandleFactory(const ModuleConfig& module_config,
                      std::unique_ptr<SinkHandle>* module_handle) {
  assert(!module_config.name.empty() && "Invalid module name");
  if (module_config.path.empty() ||
      module_config.path.find(".so") != string::npos) {
    return CppEventHandleFactory(module_config, module_handle);
  }

  OAK_ERROR("Open module %s: Unidentified module\n",
            module_config.name.c_str());
  return -1;
}

}  // namespace oak
