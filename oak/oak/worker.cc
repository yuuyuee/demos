// Copyright 2022 The Oak Authors.

#include <unistd.h>

#include "oak/addons/public/compiler.h"
#include "oak/addons/public/platform.h"
#include "oak/common/fs.h"
#include "oak/logging/logging.h"
#include "oak/config.h"

int main(int argc, char* argv[]) {
  IGNORE_UNUESD(argc, argv);

  // Worker process inherit file lock of the pid_file of the master process
  // Setup a signal to sense what time master process is exited

  OAK_DEBUG("Worker: Change working directory to %s\n",
            oak::GetCurrentDirectory().c_str());

  // Initialize process configuration
  oak::ProcessConfig process_config;
  oak::InitProcessConfig(&process_config);

  oak::WorkerConfig worker_config;
  oak::InitWorkerConfig(&worker_config, process_config.config_file);

  // Setup log config
  // oak::CreateDirectoryRecursively(process_config.log_dir.c_str());

  // Startup an channel to communicate with master process
  // When channel has established, acqurie the remaining CPU information
  // and ues for allocate the work threads, eventually, respond a message
  // to master process indicated that is done.

  while (true)
    sleep(2);

  return 0;
}
