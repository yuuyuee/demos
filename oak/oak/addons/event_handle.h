// Copyright 2022 The Oak Authors.

#ifndef OAK_ADDONS_EVENT_HANDLE_H_
#define OAK_ADDONS_EVENT_HANDLE_H_

#include <string>
#include <memory>

#include "oak/config.h"
#include "oak/addons/module.h"
#include "oak/addons/public/event.h"

namespace oak {

// EventHandle

// This class provides resource management and interface forwarding for
// event module implements.

class EventHandle: public ModuleBase {
 public:
  virtual ~EventHandle();

  EventHandle(EventHandle const&) = delete;
  EventHandle& operator=(EventHandle const&) = delete;

  // Pull out the previous event from event module.
  virtual int Pull(struct incoming_event* event, int size) = 0;

  // Receive the event from event module.
  virtual int Recv(struct incoming_event* event) = 0;

  // Send the event to event module.
  virtual int Send(const struct outgoing_event* event) = 0;

 protected:
  EventHandle(size_t id, const std::string& name, const std::string& path);
};

// Create a event handle, return 0 on success, -1 if any error
// occursed.
int EventHandleFactory(const ModuleConfig& module_config,
                       std::unique_ptr<EventHandle>* module_handle);
}  // namespace oak

#endif  // OAK_ADDONS_EVENT_HANDLE_H_
