// Copyright 2022 The Oak Authors.

#ifndef OAK_ADDONS_MODULE_H_
#define OAK_ADDONS_MODULE_H_

#include <string>

#include "oak/addons/public/parser.h"
#include "oak/addons/public/source.h"
#include "oak/addons/public/sink.h"
#include "oak/addons/public/event.h"

extern "C" {
#ifndef OAK_NAME_MAX
# define OAK_NAME_MAX 128
#endif

// OAK module header is useful to identify the types of
// the module when it should be loading.
struct oak_common_header {
  int version;
  int flags;
};

}  // extern "C"

namespace oak {

// Module type
enum ModuleType {
  MODULE_TYPE_UNKNOWN = 0,
  MODULE_TYPE_SOURCE = OAK_MODULE_SOURCE,
  MODULE_TYPE_PARSER = OAK_MODULE_PARSER,
  MODULE_TYPE_SINK = OAK_MODULE_SINK,
  MODULE_TYPE_EVENT = OAK_MODULE_EVENT
};

// Module language type
enum LangType {
  LANG_TYPE_UNKNOWN = 0,
  LANG_TYPE_C_CPP = 1,
  LANG_TYPE_PYTHON = 2,
};

// NOTE: This class MUST be default copyable.
struct Module {
  int id;
  char name[OAK_NAME_MAX];
  char path[PATH_MAX];
  ModuleType type;
  LangType lang_type;

  // For c/c++ dynamic library
  union {
    const struct oak_common_header* header;
    const struct oak_source_module* source;
    const struct oak_parser_module* parser;
    const struct oak_sink_module* sink;
    const struct oak_event_module* event;
  };
  void* dl_handler;

  // For python script
  std::string script;
};

// Initialize the module object.
void InitModule(Module* module);

// Loading module with entry @name from the path, if path has empty,
// loading from main program.
int OpenModule(Module* module, int id,
               const std::string& name,
               const std::string& path);

// Close the module to free handler.
void CloseModule(Module* module);

}  // namespace oak

#endif  // OAK_ADDONS_MODULE_H_
