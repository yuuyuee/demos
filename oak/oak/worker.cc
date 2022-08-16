// Copyright 2022 The Oak Authors.

#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>

#include "oak/config.h"
#include "oak/common/format.h"
#include "oak/common/system.h"
#include "oak/common/throw_delegate.h"
#include "oak/logging/logging.h"

namespace oak {

struct WorkerContext {
  pthread_t self;
  const LogicCore* logic_core;
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

void CreateWorker(const Config& config, CpuLayout* layout) {
  for (int i = 0; i < config.source.num_threads; ++i) {
    LogicCore* logic_core = System::GetNextAvailableCore(layout);
    if (logic_core == nullptr)
      ThrowStdOutOfRange("No enough available CPU");
    ThreadArguments args;
    args.name = oak::Format("source-%d", i);
    args.favor = logic_core->mask;
    args.routine = DummyFun;
    oak::System::CreateThread(args);
  }

  for (int i = 0; i < config.parser.num_threads; ++i) {
    LogicCore* logic_core = System::GetNextAvailableCore(layout);
    if (logic_core == nullptr)
      ThrowStdOutOfRange("No enough available CPU");
    ThreadArguments args;
    args.name = oak::Format("parser-%d", i);
    args.favor = logic_core->mask;
    args.routine = DummyFun;
    oak::System::CreateThread(args);
  }

  for (int i = 0; i < config.sink.num_threads; ++i) {
    LogicCore* logic_core = System::GetNextAvailableCore(layout);
    if (logic_core == nullptr)
      ThrowStdOutOfRange("No enough available CPU");
    ThreadArguments args;
    args.name = oak::Format("sink-%d", i);
    args.favor = logic_core->mask;
    args.routine = DummyFun;
    oak::System::CreateThread(args);
  }
}

}  // namespace oak
