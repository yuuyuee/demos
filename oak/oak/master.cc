// Copyright 2022 The Oak Authors.

#include <sys/types.h>
#include <unistd.h>

#include "oak/addons/internal/compiler.h"
#include "oak/addons/internal/platform.h"
#include "oak/common/trivial.h"
#include "oak/logging/logging.h"
#include "oak/config.h"

#define PROGRAM_NAME "master"

void CreateSlave(oak::Config&) {}
void EventLoop() {
  while (true) {
    sleep(2);
  }
}

// can not creating multiple processes with boot many process, because
// the number of processes is configured in configuration.

int main(int argc, char* argv[]) {
  if (oak::AlreadyRunning(PROGRAM_NAME)) {
    OAK_ERROR("Already running.\n");
    return 0;
  }

  oak::Config config("setup.json");

  // Logger handler = {};
  // RegisterLogger(handler);

  CreateSlave(config);

  EventLoop();
  return 0;
}
