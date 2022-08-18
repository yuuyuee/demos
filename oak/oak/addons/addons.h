// Copyright 2022 The Oak Authors.

#ifndef OAK_ADDONS_ADDONS_H_
#define OAK_ADDONS_ADDONS_H_

#include <sched.h>
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

class alignas(OAK_CACHELINE_SIZE) SourceRoutine {
 public:
  SourceRoutine();
  virtual ~SourceRoutine();

  virtual void Start() = 0;
  virtual void Stop() = 0;

  virtual void Add(const Module& module, const Dict& config) = 0;
  virtual void Routine(const cpu_set_t& mask) = 0;
};

class alignas(OAK_CACHELINE_SIZE) ParserRoutine {
 public:
  ParserRoutine();
  virtual ~ParserRoutine();

  virtual void Start() = 0;
  virtual void Stop() = 0;

  virtual void Add(const Module& module, const Dict& config) = 0;
  virtual void Remove(const Module& module) = 0;
  virtual void Update(const Module& module, const Dict& config) = 0;
  virtual void Routine(const cpu_set_t& mask) = 0;
};

class alignas(OAK_CACHELINE_SIZE) SinkRoutine {
 public:
  SinkRoutine();
  virtual ~SinkRoutine();

  virtual void Start() = 0;
  virtual void Stop() = 0;

  virtual void Add(const Module& module, const Dict& config) = 0;
  virtual void Routine(const cpu_set_t& mask) = 0;
};

// Routine context.

struct alignas(OAK_CACHELINE_SIZE) ParserContext {
  Ring report;
  pthread_spinlock_t report_lock;

  Ring metrics;
  pthread_spinlock_t metrics_lock;
};



}  // namespace oak

#endif  // OAK_ADDONS_ADDONS_H_
