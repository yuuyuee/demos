// Copyright 2022 The Oak Authors.

#include <sys/types.h>
#include <unistd.h>

#include "oak/addons/public/compiler.h"
#include "oak/addons/public/platform.h"
#include "oak/common/trivial.h"
#include "oak/logging/logging.h"
#include "oak/config.h"

// can not creating multiple processes with boot many process, because
// the number of processes is configured in configuration.

int main(int argc, char* argv[]) {
  // if (oak::AlreadyRunning(PROGRAM_NAME)) {
  //   OAK_ERROR("Already running.\n");
  //   return 0;
  // }

  if (argc != 2) {
    OAK_ERROR("Usage: %s setup.json\n", argv[0]);
    return -1;
  }

  const char* fname = argv[1];
  oak::MasterConfig master_config;
  oak::LoadMasterConfig(&master_config, fname);

  // change workding directory
  int ret = chdir()

  // Logger handler = {};
  // RegisterLogger(handler);

  // waitting for slaver is running
  // entry event loop to waitting for task event

  while (true)
    sleep(2);
  return 0;
}
