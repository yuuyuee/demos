// Copyright 2022 The Oak Authors.

#ifndef OAK_ADDONS_SOURCE_HANDLE_H_
#define OAK_ADDONS_SOURCE_HANDLE_H_

#include <string>
#include <memory>
#include <unordered_map>
#include "oak/common/macros.h"
#include "oak/addons/module.h"

namespace oak {

using Dict = std::unordered_map<std::string, std::string>;

// SourceHandle

// This class provides resource management and interface forwarding for
// source module implements.

class SourceHandle: public ModuleBase {
 public:
  SourceHandle();
  virtual ~SourceHandle();

  // Read the buffer and decoded as the metadata.
  virtual int Read(struct oak_metadata* metadata);

 private:
  OAK_DISALLOW_COPY_AND_ASSIGN(SourceHandle);
};

// Create a source module, return 0 on success, -1 if any error
// occursed.
int SourceHandleFactory(const std::string& module_name,
                        const std::string& module_path,
                        const Dict& config,
                        std::unique_ptr<SourceHandle>* module);

}  // namespace oak

#endif  // OAK_ADDONS_SOURCE_HANDLE_H_
