// Copyright 2022 The Oak Authors.

#include <unistd.h>
#include <pthread.h>
#include <string.h>

#include "oak/config.h"
#include "oak/common/format.h"
#include "oak/common/system.h"

namespace oak {

struct WorkerContext {
  pthread_t self;
  LogicCore* logic_core;
  void* caller;
  void* message;
};

void DummyFun() {
  while (true)
    sleep(2);
}

LogicCore* GetNextAvailableCore(LogicCore* core, int size) {
  for (int i = 0; i < size; ++i) {
    if (core[i].enable && !core[i].lock)
      return core + i;
  }
  return nullptr;
}

void CreateWorker(const Config& config, LogicCore* core, int size) {
  for (int i = 0; i < config.source.num_threads; ++i) {
    LogicCore* avail_core = GetNextAvailableCore(core, size);
    avail_core->lock = true;
    ThreadArguments args;
    args.name = oak::Format("source-%d", i);
    args.favor = avail_core->mask;
    args.routine = DummyFun;
    oak::System::CreateThread(args);
  }

  for (int i = 0; i < config.parser.num_threads; ++i) {
    LogicCore* avail_core = GetNextAvailableCore(core, size);
    avail_core->lock = true;
    ThreadArguments args;
    args.name = oak::Format("parser-%d", i);
    args.favor = avail_core->mask;
    args.routine = DummyFun;
    oak::System::CreateThread(args);
  }

  for (int i = 0; i < config.sink.num_threads; ++i) {
    LogicCore* avail_core = GetNextAvailableCore(core, size);
    avail_core->lock = true;
    ThreadArguments args;
    args.name = oak::Format("sink-%d", i);
    args.favor = avail_core->mask;
    args.routine = DummyFun;
    oak::System::CreateThread(args);
  }
}

}  // namespace oak
