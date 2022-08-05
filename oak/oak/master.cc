// Copyright 2022 The Oak Authors.

#include <unistd.h>

#include "oak/addons/public/compiler.h"
#include "oak/addons/public/platform.h"
#include "oak/common/trivial.h"
#include "oak/logging/logging.h"
#include "oak/config.h"

int main(int argc, char* argv[]) {
  IGNORE_UNUESD(argc);

  // Changes workding directory
  std::string self = oak::DirectoryName(argv[0]);
  oak::ChangeWorkDirectory(self.c_str());
  OAK_DEBUG("Master: Change working directory to %s\n",
            oak::GetCurrentDirectory().c_str());

  // Initialize process configuration
  oak::ProcessConfig process_config;
  oak::InitProcessConfig(&process_config);

  oak::MasterConfig master_config;
  oak::InitMasterConfig(&master_config, process_config.config_file);

  // Locks pid file to checking whther process is running
  std::string pid_file = process_config.bin_dir + "/" + master_config.pid_name;
  if (oak::AlreadyRunning(pid_file.c_str())) {
    OAK_ERROR("OAK process is already running.\n");
    return 0;
  }

  // Setup log config
  oak::CreateDirectoryRecursively(process_config.log_dir.c_str());

  // Initialize runtime environment, e.g. CPU, channel

  // Startup worker process

  // Waitting for response of the worker process

  // Initialize event receiver and waiting for task event of the outside

  while (true)
    sleep(2);

  return 0;
}
