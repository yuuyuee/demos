// Copyright 2022 The Oak Authors.

#ifndef OAK_ADDONS_ADDONS_H_
#define OAK_ADDONS_ADDONS_H_

#include <limits.h>
#include <pthread.h>

#include <atomic>
#include <string>
#include <unordered_map>

#include "oak/common/ring.h"
#include "oak/common/macros.h"
#include "oak/addons/module.h"
#include "oak/config.h"

namespace oak {
using Dict = std::unordered_map<std::string, std::string>;

struct ModuleContext {
  const Module* module;
  Dict config;
  void* priv_data;
};

class alignas(OAK_CACHELINE_SIZE) SourceRoutine {
 public:
  SourceRoutine();

  void Update(const Module& module, const Dict& config);
  void operator()() {}

 private:
  std::unordered_map<std::string, std::string> config_;
};

class alignas(OAK_CACHELINE_SIZE) ParserRoutine {
 public:
  ParserRoutine();

  void operator()() {}

 private:

};

class alignas(OAK_CACHELINE_SIZE) SinkRoutine {
 public:
  SinkRoutine();

  void operator()() {}

 private:

};


enum StopFlags {
  KEEP_RUNNING = 0,
  REQUIRE_STOP,
  ALREADY_STOPPED
};

// Routine context.

struct alignas(OAK_CACHELINE_SIZE) ParserContext {
  using Module = ModuleWrapper<oak_module_parser>;

  std::atomic<int> flags;

  std::unordered_map<int, Module> modules;
  pthread_spinlock_t module_lock;

  Ring report;
  pthread_spinlock_t report_lock;

  Ring metrics;
  pthread_spinlock_t metrics_lock;
};



}  // namespace oak

#endif  // OAK_ADDONS_ADDONS_H_
