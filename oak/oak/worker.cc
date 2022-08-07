// Copyright 2022 The Oak Authors.

#include <unistd.h>
#include <assert.h>

#include "oak/addons/public/compiler.h"
#include "oak/addons/public/platform.h"
#include "oak/common/fs.h"
#include "oak/logging/logging.h"
#include "oak/config.h"

int main(int argc, char* argv[]) {
  IGNORE_UNUESD(argc, argv);

  // Setup a signal to sense what time master process is exited.

  // Changes working directory.
  std::string path = oak::DirectoryName(argv[0]);
  oak::ChangeWorkDirectory(path);

  // Initialize process configuration
  const oak::ProcessConfig& proc_config = oak::GetWorkerProcessConfig();

  OAK_DEBUG("%s: Change working directory to %s\n",
            proc_config.proc_name.c_str(),
            oak::GetCurrentDirectory().c_str());
  assert(proc_config.bin_dir == oak::GetCurrentDirectory());

  // Master process is already locking the pid file and it is inherited
  // to the worker process.

  oak::WorkerConfig worker_config;
  oak::ReadWorkerConfig(&worker_config, proc_config.conf_file);

  oak::WriteWorkerConfig(worker_config, "worker.writeback.json");
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
