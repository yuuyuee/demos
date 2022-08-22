// Copyright 2022 The Oak Authors.

#ifndef OAK_ADDONS_SINK_HANDLE_H_
#define OAK_ADDONS_SINK_HANDLE_H_

#include <string>
#include <memory>
#include <unordered_map>
#include "oak/common/macros.h"
#include "oak/addons/module.h"

namespace oak {

using Dict = std::unordered_map<std::string, std::string>;

// SinkHandle

// This class provides resource management and interface forwarding for
// sink module implements.

class SinkHandle: public ModuleBase {
 public:
  SinkHandle();
  virtual ~SinkHandle();

  // Write the metadata to module specified the sink.
  virtual int Write(const struct oak_metadata* metadata) = 0;

 private:
  OAK_DISALLOW_COPY_AND_ASSIGN(SinkHandle);
};

// Create a sink handle, return 0 on success, -1 if any error
// occursed.
int SinkHandleFactory(const std::string& module_name,
                      const std::string& module_path,
                      const Dict& config,
                      std::unique_ptr<SinkHandle>* module);

}  // namespace oak

#endif  // OAK_ADDONS_SINK_HANDLE_H_
