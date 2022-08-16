// Copyright 2022 The Oak Authors.

#ifndef OAK_ADDONS_ADDONS_H_
#define OAK_ADDONS_ADDONS_H_

#include <limits.h>
#include <pthread.h>
#include <type_traits>
#include <atomic>
#include <unordered_map>

#include "oak/common/ring.h"
#include "oak/common/macros.h"
#include "oak/addons/public/parser.h"
#include "oak/addons/public/source.h"
#include "oak/addons/public/sink.h"


extern "C" {

// OAK module common header is useful to identify the types of
// the module when it should be loading.
struct oak_module {
  char name[OAK_NAME_MAX];
  int version;
  int flag;
  void* priv_data;
};

}   // extern "C"

namespace oak {

// Module language type
enum LangType {
  LANG_TYPE_C_CPP = 0,
  LANG_TYPE_PYTHON = 1,
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
struct ModuleWrapper {
  int id;
  char path[PATH_MAX];
  LangType lang_type;

  // For c/c++ dynamic library
  Module module;
  void* dl_handler;
};

enum StopFlags {
  KEEP_RUNNING = 0,
  REQUIRE_STOP,
  ALREADY_STOPPED
};

// Routine context.
struct alignas(OAK_CACHELINE_SIZE) SourceContext {
  using Module = ModuleWrapper<oak_source_module>;

  std::atomic<int> flags;
  std::unordered_map<int, Module> modules;
  pthread_spinlock_t module_lock;
};

struct alignas(OAK_CACHELINE_SIZE) ParserContext {
  using Module = ModuleWrapper<oak_parser_module>;

  std::atomic<int> flags;

  std::unordered_map<int, Module> modules;
  pthread_spinlock_t module_lock;

  Ring report;
  pthread_spinlock_t report_lock;

  Ring metrics;
  pthread_spinlock_t metrics_lock;
};

struct alignas(OAK_CACHELINE_SIZE) SinkContext {
  using Module = ModuleWrapper<oak_sink_module>;

  pthread_t pid;
  std::atomic<int> flags;
  std::unordered_map<int, Module> modules;
  pthread_spinlock_t module_lock;
};

// Source module routine.
void SourceRoutine(SourceContext* context);

// Parser module routine.
void ParserRoutine(ParserContext* context);

// Sink module routine.
void SinkRoutine(SinkContext* context);

}  // namespace oak

#endif  // OAK_ADDONS_ADDONS_H_
