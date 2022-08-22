// Copyright 2022 The Oak Authors.

#include "oak/addons/parser_handle.h"

namespace oak {

// ParserHandle

ParserHandle::ParserHandle() {}
ParserHandle::~ParserHandle() {}

// CppParserHandle

// Cpp implemented parser module.

class CppParserHandle: public ParserHandle {
 public:
  CppParserHandle();
  virtual ~CppParserHandle();

  // Parse stream to extract the fields.
  virtual int Parse(const struct oak_buffer_ref* up_stream,
                    const struct oak_buffer_ref* down_stream,
                    struct oak_dict* fields);

  // Parse stream to indicate whether or not the stream should
  // be controlled.
  virtual int Mark(const struct oak_buffer_ref* up_stream,
                   const struct oak_buffer_ref* down_stream);

 private:
  OAK_DISALLOW_COPY_AND_ASSIGN(CppParserHandle);
};

// PyParserHandle

// Python implemented parser module.

class PyParserHandle: public ParserHandle {
 public:
  PyParserHandle();
  virtual ~PyParserHandle();

  // Parse stream to extract the fields.
  virtual int Parse(const struct oak_buffer_ref* up_stream,
                    const struct oak_buffer_ref* down_stream,
                    struct oak_dict* fields);

  // Parse stream to indicate whether or not the stream should
  // be controlled.
  virtual int Mark(const struct oak_buffer_ref* up_stream,
                   const struct oak_buffer_ref* down_stream);

 private:
  OAK_DISALLOW_COPY_AND_ASSIGN(PyParserHandle);
};

// Create a parser handle, return 0 on success, -1 if any error
// occursed.
int ParserHandleFactory(const std::string& module_name,
                        const std::string& module_path,
                        const Dict& config,
                        std::unique_ptr<ParserHandle>* module) {

}

}  // namespace oak
