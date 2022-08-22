// Copyright 2022 The Oak Authors.

#ifndef OAK_ADDONS_ADDONS_H_
#define OAK_ADDONS_ADDONS_H_

#include <string>



#include "oak/addons/module.h"

namespace oak {

using Dict = std::unordered_map<std::string, std::string>;

// EventModuleAccessor

// This class provides resource management and interface forwarding for
// event module implements.

class EventModuleProxy {
 public:
  EventModuleProxy();
  ~EventModuleProxy();

  // Initialize the event proxy, return 0 on success, -1 if any error
  // occursed.
  static int Create(const std::string& module_name,
                    const Dict& config,
                    std::unique_ptr<EventModuleProxy>* event);

  // pull out the previous event from event module.
  int Pull(struct incoming_event* event, int size);

  // receive the event from event module.
  int Recv(struct incoming_event* event);

  // send the event to event module.
  int Send(const struct outgoing_event* event);

 private:


  EventModuleProxy(EventModuleProxy const&) = delete;
  const EventModuleProxy& operator=(EventModuleProxy const&) = delete;
};

// EventProxy

// This class provides resource management and interface forwarding for
// event module implements.

class EventProxy {
 public:
  ~EventProxy();

  // Initialize the event proxy, return 0 on success, -1 if any error
  // occursed.
  static int Create(const std::string& module_name,
                    const Dict& config,
                    std::unique_ptr<EventProxy>* event);

  // pull out the previous event from event module.
  int Pull(struct incoming_event* event, int size);

  // receive the event from event module.
  int Recv(struct incoming_event* event);

  // send the event to event module.
  int Send(const struct outgoing_event* event);

 private:
  EventProxy(const Module& module, void* context)
      : module_(module), context_(context) {}

  EventProxy(EventProxy const&) = delete;
  const EventProxy& operator=(EventProxy const&) = delete;

  Module module_;
  void* context_;
};

// EventProxy

// This class provides resource management and interface forwarding for
// event module implements.

class EventProxy {
 public:
  ~EventProxy();

  // Initialize the event proxy, return 0 on success, -1 if any error
  // occursed.
  static int Create(const std::string& module_name,
                    const Dict& config,
                    std::unique_ptr<EventProxy>* event);

  // pull out the previous event from event module.
  int Pull(struct incoming_event* event, int size);

  // receive the event from event module.
  int Recv(struct incoming_event* event);

  // send the event to event module.
  int Send(const struct outgoing_event* event);

 private:
  EventProxy(const Module& module, void* context)
      : module_(module), context_(context) {}

  EventProxy(EventProxy const&) = delete;
  const EventProxy& operator=(EventProxy const&) = delete;

  Module module_;
  void* context_;
};

// EventProxy

// This class provides resource management and interface forwarding for
// event module implements.

class EventProxy {
 public:
  ~EventProxy();

  // Initialize the event proxy, return 0 on success, -1 if any error
  // occursed.
  static int Create(const std::string& module_name,
                    const Dict& config,
                    std::unique_ptr<EventProxy>* event);

  // pull out the previous event from event module.
  int Pull(struct incoming_event* event, int size);

  // receive the event from event module.
  int Recv(struct incoming_event* event);

  // send the event to event module.
  int Send(const struct outgoing_event* event);

 private:
  EventProxy(const Module& module, void* context)
      : module_(module), context_(context) {}

  EventProxy(EventProxy const&) = delete;
  const EventProxy& operator=(EventProxy const&) = delete;

  Module module_;
  void* context_;
};

}  // namespace oak

#endif  // OAK_ADDONS_ADDONS_H_
