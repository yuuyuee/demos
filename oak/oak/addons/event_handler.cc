// Copyright 2022 The Oak Authors.

#include "oak/addons/event_handle.h"

namespace oak {

// EventHandle

EventHandle::EventHandle() = default;
EventHandle::~EventHandle() = default;

// CppEventModule

// Cpp implemented event module.

class CppEventHandle: public EventHandle {
 public:
  CppEventHandle();
  virtual ~CppEventHandle();

  // pull out the previous event from event module.
  virtual int Pull(struct incoming_event* event, int size);

  // Receive the event from event module.
  virtual int Recv(struct incoming_event* event);

  // Send the event to event module.
  virtual int Send(const struct outgoing_event* event);

  // Module unique ID
  virtual int id() const noexcept {
    return id_;
  }

  // Module version
  virtual int version() const noexcept {
    return version_;
  }

  // Module name also module entry
  virtual const std::string& name() const noexcept {
    return name_;
  }

  // Module path
  virtual const std::string& path() const noexcept {
    return path_;
  }

  // Module type
  virtual ModuleType type() const noexcept {
    return type_;
  }

  // Module language type
  virtual LangType lang_type() const noexcept {
    return lang_type_;
  }

 private:
  OAK_DISALLOW_COPY_AND_ASSIGN(CppEventHandle);

  int id_;
  int version_;
  std::string name_;
  std::string path_;
  ModuleType type_;
  LangType lang_type_;
  Dict config_;
  struct oak_event_module* handle_;
  void* dl_handle_;
  void* context_;
};

// Create a event handle, return 0 on success, -1 if any error
// occursed.
int EventModuleFactory(const std::string& module_name,
                       const std::string& module_path,
                       const Dict& config,
                       std::unique_ptr<EventHandle>* module) {

}

}  // namespace oak
