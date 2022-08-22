// Copyright 2022 The Oak Authors.

#include "oak/addons/source_handle.h"

namespace oak {

// SourceHandle

SourceHandle::SourceHandle() {}
SourceHandle::~SourceHandle() {}

// CppSourceHandle

// Cpp implemented source handle.

class CppSourceHandle: public SourceHandle {
 public:
  CppSourceHandle();
  virtual ~CppSourceHandle();

  // Read the buffer and decoded as the metadata.
  virtual int Read(struct oak_metadata* metadata);

 private:
  OAK_DISALLOW_COPY_AND_ASSIGN(CppSourceHandle);
};

// Create a source handle, return 0 on success, -1 if any error
// occursed.
int SourceHandleFactory(const std::string& module_name,
                        const std::string& module_path,
                        const Dict& config,
                        std::unique_ptr<SourceHandle>* module) {

}

}  // namespace oak
