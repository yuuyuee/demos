// Copyright 2022 The Oak Authors.

#ifndef OAK_ADDONS_ADDONS_H_
#define OAK_ADDONS_ADDONS_H_

#include <limits.h>
#include <mutex>  // NOLINT

#include "oak/common/ring.h"
#include "oak/common/macros.h"
#include "oak/addons/public/parser.h"
#include "oak/addons/public/source.h"
#include "oak/addons/public/sink.h"

namespace oak {

template <typename Module>
struct alignas(OAK_CACHELINE_SIZE) ModuleWrapper {
  Module module;
  char path[PATH_MAX];
  void* handler;
  Ring node;
};

struct SourceContext {
  Ring handler;
  std::mutex lock;
};

// Routine context.
struct RoutineContext {
  std::mutex lock;
  Ring report;
  Ring
};

// Source module routine.
void SourceRoutine(RoutineContext* context);

void ParserRoutine(RoutineContext* context);

void SinkRoutine(RoutineContext* context);

}  // namespace oak

#endif  // OAK_ADDONS_ADDONS_H_
