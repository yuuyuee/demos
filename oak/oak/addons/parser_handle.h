// Copyright 2022 The Oak Authors.

#ifndef OAK_ADDONS_PARSER_HANDLE_H_
#define OAK_ADDONS_PARSER_HANDLE_H_

#include <string>
#include <memory>

#include "oak/config.h"
#include "oak/addons/module.h"
#include "oak/addons/public/parser.h"

namespace oak {

// ParserHandle

// This class provides resource management and interface forwarding for
// parser module implements.

class ParserHandle: public ModuleBase {
 public:
  virtual ~ParserHandle();

  ParserHandle(ParserHandle const&) = delete;
  ParserHandle& operator=(ParserHandle const&) = delete;

  // Parse stream to extracting to filling the fields.
  virtual int Parse(const struct oak_buffer* up_stream,
                    const struct oak_buffer* down_stream,
                    const struct oak_dict* extracted,
                    struct oak_dict* fields) = 0;

  // Parse stream to indicating whether or not the stream should
  // be controlled.
  virtual int Mark(const struct oak_buffer* up_stream,
                   const struct oak_buffer* down_stream) = 0;

 protected:
  ParserHandle(size_t id, const std::string& name, const std::string& path);
};

// Create a parser handle, return 0 on success, -1 if any error
// occursed.
int ParserHandleFactory(const ModuleConfig& module_config,
                        std::unique_ptr<ParserHandle>* module_handle);

}  // namespace oak

#endif  // OAK_ADDONS_PARSER_HANDLE_H_
