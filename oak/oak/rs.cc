// Copyright 2022 The Oak Authors.

#include <unistd.h>
#include <assert.h>
#include <stdio.h>

#include "oak/addons/public/compiler.h"
#include "oak/addons/public/platform.h"
#include "oak/addons/public/version.h"
#include "oak/common/macros.h"
#include "oak/common/format.h"
#include "oak/common/fs.h"
#include "oak/common/debug.h"
#include "oak/common/system.h"
#include "oak/logging/logging.h"
#include "oak/config.h"
#include "oak/event/server.h"

namespace {

#define OAK_LINE_STR(x) x "\n"
#define OAK_RS_USAGE                              \
  OAK_LINE_STR("Usage: rs <-h>")                  \
  OAK_LINE_STR("Version: " OAK_VERSION_STR)       \
  OAK_LINE_STR("Options:")                        \
  OAK_LINE_STR("    -h print help")

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

namespace oak {
void CreateWorker(const Config& config, CpuLayout* layout);
}  // namespace oak

int main(int argc, char* argv[]) {
  for (int i = 1; i < argc; ++i) {
    if (argv[i][0] == '-' &&
        argv[i][1] == 'h' &&
        argv[i][2] == '\0') {
      printf("%s", OAK_RS_USAGE);
      return 0;
    }
  }

  // Setup exception handler.
  oak::SetSignalAltStack();
  oak::RegisterFailureSignalHandler();
  oak::RegisterFailureMessageHandler(STDERR_FILENO);

  // Changes working directory.
  std::string path = oak::DirectoryName(argv[0]);
  oak::ChangeWorkDirectory(path);

  // Initialize process configuration.
  const oak::ProcessConfig& proc_config = oak::GetProcessConfig();

  // Setup crash handler.
  oak::CreateDirectory(proc_config.log_dir);
  oak::RegisterFailureMessageHandler(proc_config.crash_file);

  // Locks pid file to checking whther process is running.
  oak::CreateDirectory(oak::DirectoryName(proc_config.guard_file));
  if (!CreateGuardFile(proc_config.guard_file)) {
    // Other process has already locked.
    OAK_ERROR("%s: Process is already running.\n",
              proc_config.proc_name.c_str());
    return -1;
  }

  OAK_INFO("%s: Change working directory to %s\n",
           proc_config.proc_name.c_str(),
           oak::GetCurrentDirectory().c_str());
  assert(proc_config.bin_dir == oak::GetCurrentDirectory());

  oak::Config config;
  oak::ReadConfig(&config, proc_config.conf_file);
  oak::WriteConfig(config, proc_config.etc_dir + "/rs.write.json");

  // Setup logger.
  // TODO(YUYUE):

  // Initialize runtime environment, e.g. CPU, events, workers.

  // Initialize CPU layout.
  oak::CpuLayout layout;
  oak::System::GetCpuLayout(&layout);
  assert(layout.available_cores > 0 && "Logic error");

  int current_core = oak::System::GetCurrentCpu();
  oak::LogicCore* logic_core =
      oak::System::GetNextAvailableCore(&layout, current_core);
  assert(logic_core != nullptr && "Logic error");
  oak::System::SetThreadAffinity(pthread_self(), logic_core->mask);
  oak::System::ThreadYield();

  // Initialize events receiver.

  // Recover events that has occurred.

  // creates worker thread.
  oak::CreateWorker(config, &layout);

  // Waiting for task event of the outside.
  while (true) {
    sleep(2);
  }

  return 0;
}

