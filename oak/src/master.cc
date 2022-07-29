// Copyright 2022 The Oak Authors.

#include <unistd.h>

#include "oak/internal/compiler.h"
#include "oak/internal/platform.h"
#include "common/trivial.h"
//#include "config/config.h"

using namespace oak;

void LoadConfig();
void CreateShareMemory();
void AssignShareMemory();
void CreateWorkers();
void WaitWrokersReady();
void EventLoop();

// can not creating multiple processes with boot many process, because
// the number of processes is configured in configuration.

int main(int argc, char* argv[]) {
  if (AlreadyRunning(argv[0])) {
    OAK_RAWLOG("%s already running.\n", argv[0]);
    return 0;
  }

/*
  LoadConfig();

  CreateShareMemory();
  CreateWorkers();
  WaitWrokersReady();
  EventLoop();
  AssignShareMemory();
*/

  while (true)
    sleep(2);

  return 0;
}
