// Copyright 2022 The Oak Authors.

#include <sys/types.h>
#include <unistd.h>

#include "oak/addons/internal/compiler.h"
#include "oak/addons/internal/platform.h"
#include "oak/logging/logging.h"
#include "oak/config.h"

class FunctionDelegate {
 public:

};

void CreateSlave() {}
void WaitWrokersReady() {}
void EventLoop() {
  while (true) {
    sleep(2);
  }
}

// can not creating multiple processes with boot many process, because
// the number of processes is configured in configuration.

int main(int argc, char* argv[]) {
  if (oak::AlreadyRunning(argv[0])) {
    OAK_ERROR("Already running.\n");
    return 0;
  }

  // Logger handler = {};
  // RegisterLogger(handler);

  oak::Config config("setup.json");

  if (config.multiple_process) {

  } else {
    CreateSlave();
  }

  EventLoop();
  return 0;
}
