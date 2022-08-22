// Copyright 2022 The Oak Authors.

#ifndef OAK_ADDONS_EVENT_HANDLE_H_
#define OAK_ADDONS_EVENT_HANDLE_H_

#include <string>
#include <memory>
#include <unordered_map>
#include "oak/common/macros.h"
#include "oak/addons/module.h"

namespace oak {

using Dict = std::unordered_map<std::string, std::string>;

// EventHandle

// This class provides resource management and interface forwarding for
// event module implements.

class EventHandle: public ModuleBase {
 public:
  EventHandle();
  virtual ~EventHandle();

  // Pull out the previous event from event module.
  virtual int Pull(struct incoming_event* event, int size) = 0;

  // Receive the event from event module.
  virtual int Recv(struct incoming_event* event) = 0;

  // Send the event to event module.
  virtual int Send(const struct outgoing_event* event) = 0;

 private:
  OAK_DISALLOW_COPY_AND_ASSIGN(EventHandle);
};

// Create a event handle, return 0 on success, -1 if any error
// occursed.
int EventModuleFactory(const std::string& module_name,
                       const std::string& module_path,
                       const Dict& config,
                       std::unique_ptr<EventHandle>* module);
}  // namespace oak

#endif  // OAK_ADDONS_EVENT_HANDLE_H_
