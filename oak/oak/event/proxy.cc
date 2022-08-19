// Copyright 2022 The Oak Authors.

#include "oak/event/proxy.h"

#include "oak/common/format.h"
#include "oak/logging/logging.h"

namespace oak {

int EventProxy::Create(const std::string& module_name,
                       const Dict& config,
                       std::unique_ptr<EventProxy>* event) {
  Module module;
  InitModule(&module);
  int ret = OpenModule(&module, -1, module_name, "");
  if (ret != 0) {
    OAK_ERROR("Open module \'%s\' failed\n", module_name.c_str());
    return -1;
  }

  if (module.type != MODULE_TYPE_EVENT) {
    CloseModule(&module);
    OAK_ERROR("Module \'%s\' is not event module\n", module_name.c_str());
    return -1;
  }

  if (module.lang_type != LANG_TYPE_C_CPP) {
    CloseModule(&module);
    OAK_ERROR("Module \'%s\' is not support\n", module_name.c_str());
    return -1;
  }

  struct oak_dict module_config;
  oak_dict_init(&module_config);
  for (auto const& it : config) {
    if (module_config.size >= OAK_DICT_DFL_CAP - 1)
      break;
    if (it.first.empty() || it.second.empty())
      continue;
    struct oak_buffer* key = &(module_config.elems[module_config.size].key);
    struct oak_buffer* value = &(module_config.elems[module_config.size].value);
    oak_buffer_assign_str(key, it.first.c_str(), it.first.size());
    oak_buffer_assign_str(value, it.second.c_str(), it.second.size());
    ++module_config.size;
  }

  // there do not assume that context should not be nullptr, because some
  // module may not context.
  void* context = module.event->init(&module_config);
  event->reset(new EventProxy(module, context));
  oak_dict_free(&module_config);
  return 0;
}

EventProxy::~EventProxy() {
  module_.event->close(context_);
  CloseModule(&module_);
}

int EventProxy::Pull(struct incoming_event* event, int size) {
  return module_.event->pull(context_, event, size);
}


int EventProxy::Recv(struct incoming_event* event) {
  return module_.event->recv(context_, event);
}

int EventProxy::Send(const struct outgoing_event* event) {
  return module_.event->send(context_, event);
}

}  // namespace oak
