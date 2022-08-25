// Copyright 2022 The Oak Authors.

#include "oak/addons/event_handle.h"

#include <assert.h>
#include <dlfcn.h>
#include "oak/addons/dict_internal.h"
#include "oak/logging/logging.h"

using std::string;
using std::unique_ptr;

namespace oak {

// EventHandle

EventHandle::EventHandle(size_t id, const string& name, const string& path)
    : ModuleBase(id, name, path) {}

EventHandle::~EventHandle() {}

// CppEventModule

// Cpp implemented event module.

class CppEventHandle: public EventHandle {
 public:
  explicit CppEventHandle(size_t id,
                          const string& name,
                          const string& path,
                          void* dl_handle,
                          struct oak_event_module* handle,
                          void* context);

  virtual ~CppEventHandle();

  CppEventHandle(CppEventHandle const&) = delete;
  CppEventHandle& operator=(CppEventHandle const&) = delete;

  // pull out the previous event from event module.
  virtual int Pull(struct incoming_event* event, int size) {
    return handle_->pull(context_, event, size);
  }

  // Receive the event from event module.
  virtual int Recv(struct incoming_event* event) {
    return handle_->recv(context_, event);
  }

  // Send the event to event module.
  virtual int Send(const struct outgoing_event* event) {
    return handle_->send(context_, event);
  }

 private:
  void* dl_handle_;
  struct oak_event_module* handle_;
  void* context_;
};

CppEventHandle::CppEventHandle(size_t id,
                               const string& name,
                               const string& path,
                               void* dl_handle,
                               struct oak_event_module* handle,
                               void* context)
    : EventHandle(id, name, path),
      dl_handle_(dl_handle), handle_(handle), context_(context) {
  assert(!name.empty() && "Invalid module name");
  assert(dl_handle && "Invalid dl_handle");
  assert(handle && "Invalid handle");
  version_ = handle_->version;
  type_ = MODULE_TYPE_EVENT;
  lang_type_ = LANG_TYPE_C_CPP;
}

CppEventHandle::~CppEventHandle() {
  handle_->close(context_);
  dlerror();
  if (dlclose(dl_handle_) != 0) {
    OAK_WARNING("Close module %s: dlclose failed: %s\n",
                name_.c_str(), dlerror());
  }
}

int CppEventHandleFactory(const ModuleConfig& module_config,
                          unique_ptr<EventHandle>* module_handle) {
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

  struct oak_event_module* handle =
      reinterpret_cast<struct oak_event_module*>(symbol);

  if (handle->flags != MODULE_TYPE_EVENT) {
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

  CppEventHandle* object =
      new CppEventHandle(module_config.id,
                         module_config.name,
                         module_config.path,
                         dl_handle, handle, context);
  object->Dump();
  module_handle->reset(object);
  return 0;
}

// Create a event handle, return 0 on success, -1 if any error
// occursed.
int EventHandleFactory(const ModuleConfig& module_config,
                       unique_ptr<EventHandle>* module_handle) {
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
