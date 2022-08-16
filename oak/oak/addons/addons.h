// Copyright 2022 The Oak Authors.

#ifndef OAK_ADDONS_ADDONS_H_
#define OAK_ADDONS_ADDONS_H_

#include <limits.h>
#include <mutex>  // NOLINT
#include <type_traits>

#include "oak/common/ring.h"
#include "oak/common/macros.h"
#include "oak/addons/public/parser.h"
#include "oak/addons/public/source.h"
#include "oak/addons/public/sink.h"


extern "C" {

// OAK module common header is useful to identify the types of
// the module when it should be loading.
struct ModuleHeader {
  char name[OAK_NAME_MAX];
  int version;
  int flag;
  void* priv_data;
};

}   // extern "C"

static_assert(offsetof(ModuleHeader, priv_data) ==
              offsetof(oak_source_module, priv_data),
              "logic error");
static_assert(offsetof(ModuleHeader, priv_data) ==
              offsetof(oak_parser_module, priv_data),
              "logic error");
static_assert(offsetof(ModuleHeader, priv_data) ==
              offsetof(oak_sink_module, priv_data),
              "logic error");

namespace oak {

// Module language type
enum LangType {
  LANG_TYPE_C_CPP = 0,
};

template <typename Module>
struct IsModule: std::false_type {};

template <> struct IsModule<oak_source_module>: std::true_type {};
template <> struct IsModule<oak_parser_module>: std::true_type {};
template <> struct IsModule<oak_sink_module>: std::true_type {};

// There template must is one of the following type:
// oak_source_module, oak_parser_module, oak_sink_module
template <typename Module,
          typename = std::enable_if<IsModule<Module>::value>>
struct alignas(OAK_CACHELINE_SIZE) ModuleWrapper {
  int id;
  char path[PATH_MAX];
  LangType lang_type;

  // For c/c++ dynamic library
  Module module;
  void* dl_handler;
};

// Routine context.
struct RoutineContext {
  std::mutex lock;
  ModuleWrapper<oak_source_module> a;
  Ring report;
  Ring metrics;
};

class SourceContext {
 public:
  SourceContext();
};

// Source module routine.
void SourceRoutine(RoutineContext* context);

void ParserRoutine(RoutineContext* context);

void SinkRoutine(RoutineContext* context);

}  // namespace oak

#endif  // OAK_ADDONS_ADDONS_H_
