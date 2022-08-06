// Copyright 2022 The Oak Authors.

#include <unistd.h>

#include "oak/addons/public/compiler.h"
#include "oak/addons/public/platform.h"
#include "oak/common/trivial.h"
#include "oak/common/fs.h"
#include "oak/logging/logging.h"
#include "oak/config.h"

int main(int argc, char* argv[]) {
  IGNORE_UNUESD(argc);

  // Changes workding directory.
  std::string path = oak::DirectoryName(argv[0]);
  oak::ChangeWorkDirectory(path);

  // Initialize process configuration.
  const oak::ProcessConfig& proc_config = oak::GetMasterProcessConfig();
  OAK_INFO("%s: Change working directory to %s\n",
           proc_config.proc_name.c_str(),
           proc_config.bin_dir.c_str());

  // Locks pid file to checking whther process is running.
  oak::CreateDirectory(oak::DirectoryName(proc_config.pid_file));
  if (oak::AlreadyRunning(proc_config.pid_file)) {
    OAK_ERROR("OAK process is already running.\n");
    return 0;
  }

  oak::MasterConfig master_config;
  oak::ReadMasterConfig(&master_config, proc_config.conf_file);


  oak::WriteMasterConfig(master_config, "master.writeback.json");

  // Setup log config.
  oak::CreateDirectory(proc_config.log_dir);

  // Initialize runtime environment, e.g. CPU, channel.

  // Startup worker process.

  // Waitting for response of the worker process.

  // Initialize event receiver and waiting for task event of the outside.

  while (true)
    sleep(2);

  return 0;
}
