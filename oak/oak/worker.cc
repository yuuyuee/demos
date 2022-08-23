// Copyright 2022 The Oak Authors.

#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>

#include "oak/config.h"
#include "oak/common/format.h"
#include "oak/common/system.h"
#include "oak/common/throw_delegate.h"
#include "oak/common/channel.h"
#include "oak/logging/logging.h"

namespace oak {

enum Role {
  ROLE_SOURCE,
  ROLE_PARSER,
  ROLE_SINK,
  ROLE_UNKNOWN
};

struct WorkerContext {
  pthread_t self;
  Role role;
  const LogicCore* logic_core;
  void* caller;
  void* message;
};

void DummyFun() {
  while (true)
    sleep(2);
}

void CreateWorker(const Config& config) {
  // for (int i = 0; i < config.source.num_threads; ++i) {
  //   LogicCore* logic_core = System::GetNextAvailableLogicCore();
  //   if (logic_core == nullptr)
  //     ThrowStdOutOfRange("No enough available CPU");
  //   oak::System::CreateThread(oak::Format("source-%d", i),
  //                             logic_core->mask,
  //                             DummyFun);
  // }

  // for (int i = 0; i < config.parser.num_threads; ++i) {
  //   LogicCore* logic_core = System::GetNextAvailableLogicCore();
  //   if (logic_core == nullptr)
  //     ThrowStdOutOfRange("No enough available CPU");
  //   oak::System::CreateThread(oak::Format("parser-%d", i),
  //                             logic_core->mask,
  //                             DummyFun);
  // }

  // for (int i = 0; i < config.sink.num_threads; ++i) {
  //   LogicCore* logic_core = System::GetNextAvailableLogicCore();
  //   if (logic_core == nullptr)
  //     ThrowStdOutOfRange("No enough available CPU");
  //   oak::System::CreateThread(oak::Format("sink-%d", i),
  //                             logic_core->mask,
  //                             DummyFun);
  // }
}

}  // namespace oak
