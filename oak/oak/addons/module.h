// Copyright 2022 The Oak Authors.

#ifndef OAK_ADDONS_MODULE_H_
#define OAK_ADDONS_MODULE_H_

#include <string>
#include <memory>
#include <unordered_map>

#include "oak/common/macros.h"
#include "oak/addons/public/parser.h"
#include "oak/addons/public/source.h"
#include "oak/addons/public/sink.h"
#include "oak/addons/public/event.h"

extern "C" {

// OAK module header is useful to identify the types of
// the module when it should be loading.
struct oak_cpp_handle {
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

constexpr const char* ModuleTypeName(ModuleType type) {
  return type == MODULE_TYPE_SOURCE ? "Source"
      : type == MODULE_TYPE_PARSER ? "Parser"
      : type == MODULE_TYPE_SINK ? "Sink"
      : type == MODULE_TYPE_EVENT ? "Event"
      : "Unknown";
}

// Module language type
enum LangType {
  LANG_TYPE_UNKNOWN = 0,
  LANG_TYPE_C_CPP = 1,
  LANG_TYPE_PYTHON = 2,
};

constexpr const char* LangTypeName(LangType type) {
  return type == LANG_TYPE_C_CPP ? "C/C++"
      : type == LANG_TYPE_PYTHON ? "Python"
      : "Unknown";
}

// ModuleHandle

// Maintain the lifecycle of the underlying handle.




using Dict = std::unordered_map<std::string, std::string>;

// EventModule

// This class provides resource management and interface forwarding for
// event module implements.

class EventModule {
 public:
  EventModule();
  virtual ~EventModule();

  // Pull out the previous event from event module.
  virtual int Pull(struct incoming_event* event, int size) = 0;

  // Receive the event from event module.
  virtual int Recv(struct incoming_event* event) = 0;

  // Send the event to event module.
  virtual int Send(const struct outgoing_event* event) = 0;

  // Get unique ID of the event module.
  virtual int id() const noexcept = 0;

  // Get unique name of the event module.
  virtual std::string name() const noexcept = 0;

 private:
  OAK_DISALLOW_COPY_AND_ASSIGN(EventModule);
};

// Create a event module, return 0 on success, -1 if any error
// occursed.
int EventModuleFactory(const std::string& module_name,
                       const Dict& config,
                       std::unique_ptr<EventModule>* module);

// SourceModule

// This class provides resource management and interface forwarding for
// source module implements.

class SourceModule {
 public:
  SourceModule();
  virtual ~SourceModule();

  // Read the buffer and decoded as the metadata.
  virtual int Read(struct oak_metadata* metadata);

  // Get unique ID of the source module.
  virtual int id() const noexcept = 0;

  // Get unique name of the source module.
  virtual std::string name() const noexcept = 0;

 private:
  OAK_DISALLOW_COPY_AND_ASSIGN(SourceModule);
};

// Create a source module, return 0 on success, -1 if any error
// occursed.
int SourceModuleFactory(const std::string& module_name,
                        const Dict& config,
                        std::unique_ptr<SourceModule>* module);

// ParserModule

// This class provides resource management and interface forwarding for
// parser module implements.

class ParserModule {
 public:
  ParserModule();
  virtual ~ParserModule();

  // Parse stream to extract the fields.
  virtual int Parse(const struct oak_buffer_ref* up_stream,
                    const struct oak_buffer_ref* down_stream,
                    struct oak_dict* fields) = 0;

  // Parse stream to indicate whether or not the stream should
  // be controlled.
  virtual int Mark(const struct oak_buffer_ref* up_stream,
                   const struct oak_buffer_ref* down_stream) = 0;

  // Get unique ID of the parser module.
  virtual int id() const noexcept = 0;

  // Get unique name of the parser module.
  virtual std::string name() const noexcept = 0;

 private:
  OAK_DISALLOW_COPY_AND_ASSIGN(ParserModule);
};

// Create a parser module, return 0 on success, -1 if any error
// occursed.
int ParserModuleFactory(const std::string& module_name,
                        const Dict& config,
                        std::unique_ptr<ParserModule>* module);

// SinkModule

// This class provides resource management and interface forwarding for
// sink module implements.

class SinkModule {
 public:
  SinkModule();
  virtual ~SinkModule();

  // Write the metadata to module specified the sink.
  virtual int Write(const struct oak_metadata* metadata) = 0;

  // Get unique ID of the sink module.
  virtual int id() const noexcept = 0;

  // Get unique name of the sink module.
  virtual std::string name() const noexcept = 0;

 private:
  OAK_DISALLOW_COPY_AND_ASSIGN(SinkModule);
};

// Create a sink module, return 0 on success, -1 if any error
// occursed.
int SinkModuleFactory(const std::string& module_name,
                      const Dict& config,
                      std::unique_ptr<SinkModule>* module);

}  // namespace oak

#endif  // OAK_ADDONS_MODULE_H_
