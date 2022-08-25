// Copyright 2022 The Oak Authors.

#ifndef OAK_ADDONS_SOURCE_HANDLE_H_
#define OAK_ADDONS_SOURCE_HANDLE_H_

#include <string>
#include <memory>

#include "oak/config.h"
#include "oak/addons/module.h"
#include "oak/addons/public/source.h"

namespace oak {

// SourceHandle

// This class provides resource management and interface forwarding for
// source module implements.

class SourceHandle: public ModuleBase {
 public:
  virtual ~SourceHandle();

  SourceHandle(SourceHandle const&) = delete;
  SourceHandle& operator=(SourceHandle const&) = delete;

  // Read the buffer and decoded as the metadata.
  virtual int Read(struct oak_metadata* metadata) = 0;

 protected:
  SourceHandle(size_t id, const std::string& name, const std::string& path);
};

// Create a source handle, return 0 on success, -1 if any error
// occursed.
int SourceHandleFactory(const ModuleConfig& module_config,
                        std::unique_ptr<SourceHandle>* module_handle);
}  // namespace oak

#endif  // OAK_ADDONS_SOURCE_HANDLE_H_
