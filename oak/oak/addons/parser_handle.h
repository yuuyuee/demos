// Copyright 2022 The Oak Authors.

#ifndef OAK_ADDONS_PARSER_HANDLE_H_
#define OAK_ADDONS_PARSER_HANDLE_H_

#include <string>
#include <memory>
#include <unordered_map>
#include "oak/common/macros.h"
#include "oak/addons/module.h"

namespace oak {

using Dict = std::unordered_map<std::string, std::string>;

// ParserHandle

// This class provides resource management and interface forwarding for
// parser module implements.

class ParserHandle: public ModuleBase {
 public:
  ParserHandle();
  virtual ~ParserHandle();

  // Parse stream to extract the fields.
  virtual int Parse(const struct oak_buffer_ref* up_stream,
                    const struct oak_buffer_ref* down_stream,
                    struct oak_dict* fields) = 0;

  // Parse stream to indicate whether or not the stream should
  // be controlled.
  virtual int Mark(const struct oak_buffer_ref* up_stream,
                   const struct oak_buffer_ref* down_stream) = 0;

 private:
  OAK_DISALLOW_COPY_AND_ASSIGN(ParserHandle);
};

// Create a parser handle, return 0 on success, -1 if any error
// occursed.
int ParserHandleFactory(const std::string& module_name,
                        const std::string& module_path,
                        const Dict& config,
                        std::unique_ptr<ParserHandle>* module);

}  // namespace oak

#endif  // OAK_ADDONS_PARSER_HANDLE_H_
