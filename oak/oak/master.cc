// Copyright 2022 The Oak Authors.

#include <unistd.h>
#include <assert.h>
#include <stdio.h>

#include "oak/addons/public/compiler.h"
#include "oak/addons/public/platform.h"
#include "oak/common/macros.h"
#include "oak/common/format.h"
#include "oak/common/fs.h"
#include "oak/common/debug.h"
#include "oak/common/system.h"
#include "oak/common/asio_wrapper.h"
#include "oak/logging/logging.h"
#include "oak/config.h"

namespace oak {
namespace {
bool CreateGuardFile(const std::string& guard_file) {
  std::string pid = oak::Format("%d", getpid());
  oak::File file = oak::File::MakeWritableFile(guard_file);
  if (!file.TryLock())
    return false;

  file.Write(pid);
  file.Sync();

  // Releases the file descriptor to prevent the close
  // on the return to keep locking.
  file.Release();
  return true;
}

}  // anonymous namespace

void Master(int argc, char* argv[]) {
  IGNORE_UNUESD(argc);

  // Setup exception handler.
  oak::SetSignalAltStack();
  oak::RegisterFailureSignalHandler();
  oak::RegisterFailureMessageHandler(STDERR_FILENO);

  // Changes working directory.
  std::string path = oak::DirectoryName(argv[0]);
  oak::ChangeWorkDirectory(path);

  // Initialize process configuration.
  const oak::ProcessConfig& proc_config = oak::GetMasterProcessConfig();

  // Setup crash handler.
  oak::CreateDirectory(proc_config.log_dir);
  oak::RegisterFailureMessageHandler(proc_config.crash_file);

  // Locks pid file to checking whther process is running.
  oak::CreateDirectory(oak::DirectoryName(proc_config.guard_file));
  if (!CreateGuardFile(proc_config.guard_file)) {
    // Other process has already locked.
    OAK_ERROR("%s: Process is already running.\n",
              proc_config.proc_name.c_str());
    return;
  }

  OAK_INFO("%s: Change working directory to %s\n",
           proc_config.proc_name.c_str(),
           oak::GetCurrentDirectory().c_str());
  assert(proc_config.bin_dir == oak::GetCurrentDirectory());

  oak::MasterConfig master_config;
  oak::ReadMasterConfig(&master_config, proc_config.conf_file);

  // Setup runtime logging and business logging.
  // TODO(YUYUE):

  // Initialize runtime environment, e.g. CPU, channel.
  oak::CpuLayout cpu_layout;
  oak::System::InitCpuLayout(&cpu_layout);

  // Startup worker process.

  // Waiting for response of the worker process.

  // Initialize event receiver and waiting for task event of the outside.

  while (true)
    sleep(2);
}

void Worker(int argc, char* argv[]);
}  // namespace oak

int main(int argc, char* argv[]) {
  const char* usage =
    "Usage: rs <-w> ...\n"
    "Options:"
    "    -w running as woker, default as master\n"
    "    -h print help\n";

  bool running_as_master = true;
  for (int i = 1; i < argc; ++i) {
    if (argv[i][0] == '-' && argv[i][1] == 'h' && argv[i][2] == '\0') {
      printf("%s", usage);
      return 0;
    } else if (argv[i][0] == '-' && argv[i][1] == 'w' && argv[i][2] == '\0') {
      running_as_master = false;
    }
  }
  running_as_master ? oak::Master(argc, argv) : oak::Worker(argc, argv);
  return 0;
}
