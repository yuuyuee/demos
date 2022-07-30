// Copyright 2022 The Oak Authors.

#include <unistd.h>

#include "oak/addons/internal/compiler.h"
#include "oak/addons/internal/platform.h"
#include "oak/logging/logging.h"
#include "oak/config.h"

void LoadConfig();
void CreateShareMemory();
void AssignShareMemory();
void CreateWorkers();
void WaitWrokersReady();
void EventLoop();

// can not creating multiple processes with boot many process, because
// the number of processes is configured in configuration.

int main(int argc, char* argv[]) {
  if (oak::AlreadyRunning(argv[0])) {
    OAK_ERROR("Already running.\n");
    return 0;
  }

  // Logger handler = {};
  // RegisterLogger(handler);

  // LoadConfig();

/*
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