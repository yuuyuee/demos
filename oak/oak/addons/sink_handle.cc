// Copyright 2022 The Oak Authors.

#include "oak/addons/sink_handle.h"

namespace oak {

// SinkHandle

SinkHandle::SinkHandle() {}
SinkHandle::~SinkHandle() {}

// CppSinkHandle

// Cpp implemented sink module.

class CppSinkHandle: public SinkHandle {
 public:
  CppSinkHandle();
  virtual ~CppSinkHandle();

  // Write the metadata to module specified the sink.
  virtual int Write(const struct oak_metadata* metadata) = 0;

 private:
  OAK_DISALLOW_COPY_AND_ASSIGN(CppSinkHandle);
};

// Create a sink handle, return 0 on success, -1 if any error
// occursed.
int SinkHandleFactory(const std::string& module_name,
                      const std::string& module_path,
                      const Dict& config,
                      std::unique_ptr<SinkHandle>* module) {

}

}  // namespace oak
