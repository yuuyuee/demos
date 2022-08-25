// Copyright 2022 The Oak Authors.

#ifndef OAK_ADDONS_SINK_HANDLE_H_
#define OAK_ADDONS_SINK_HANDLE_H_

#include <string>
#include <memory>

#include "oak/config.h"
#include "oak/addons/module.h"
#include "oak/addons/public/sink.h"

namespace oak {

// SinkHandle

// This class provides resource management and interface forwarding for
// sink module implements.

class SinkHandle: public ModuleBase {
 public:
  virtual ~SinkHandle();

  SinkHandle(SinkHandle const&) = delete;
  SinkHandle& operator=(SinkHandle const&) = delete;

  // Write the metadata to module specified the sink.
  virtual int Write(const struct oak_metadata* metadata) = 0;

 protected:
  SinkHandle(size_t id, const std::string& name, const std::string& path);
};

// Create a sink handle, return 0 on success, -1 if any error
// occursed.
int SinkHandleFactory(const ModuleConfig& module_config,
                      std::unique_ptr<SinkHandle>* module_handle);

}  // namespace oak

#endif  // OAK_ADDONS_SINK_HANDLE_H_
