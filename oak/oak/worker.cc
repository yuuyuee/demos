// Copyright 2022 The Oak Authors.

#include <unistd.h>
#include <assert.h>
#include <signal.h>

#include "oak/addons/public/compiler.h"
#include "oak/addons/public/platform.h"
#include "oak/common/macros.h"
#include "oak/common/fs.h"
#include "oak/common/format.h"
#include "oak/common/debug.h"
#include "oak/common/system.h"
#include "oak/logging/logging.h"
#include "oak/config.h"

namespace oak {
namespace {
// bool CheckGuardFile(const std::string& guard_file) {
//   std::string ppid = oak::Format("%d", getppid());
//   oak::File file = oak::File::MakeRandomAccessFile(guard_file);
//   if (file.TryLock())
//     return false;

//   file.Seek(SEEK_SET, 0);
//   char buffer[32];
//   size_t size = file.Read(buffer, sizeof(buffer));
//   return ppid.size() == size &&
//          ppid.compare(0, size, buffer, size) == 0;
// }

}  // anonymous namespace

void Worker(int argc, char* argv[]) {
  IGNORE_UNUESD(argc, argv);

  // Setup exception handler.
  // oak::System::SetParentDeathSignal(SIGTERM);
  oak::SetSignalAltStack();
  oak::RegisterFailureSignalHandler();
  oak::RegisterFailureMessageHandler(STDERR_FILENO);

  // Changes working directory.
  std::string path = oak::DirectoryName(argv[0]);
  oak::ChangeWorkDirectory(path);

  // Initialize process configuration
  const oak::ProcessConfig& proc_config = oak::GetWorkerProcessConfig();

  // Setup crash handler.
  oak::CreateDirectoryRecursively(proc_config.log_dir);
  oak::RegisterFailureMessageHandler(proc_config.crash_file);

  // Cheaks whether or not the pid file has locked by parent process.
  // if (!oak::IsExists(proc_config.guard_file) ||
  //     !CheckGuardFile(proc_config.guard_file)) {
  //   OAK_ERROR("%s: Unable to running individually.\n",
  //             proc_config.proc_name.c_str());
  //   return;
  // }

  OAK_INFO("%s: Change working directory to %s\n",
            proc_config.proc_name.c_str(),
            oak::GetCurrentDirectory().c_str());
  assert(proc_config.bin_dir == oak::GetCurrentDirectory());

  oak::WorkerConfig worker_config;
  oak::ReadWorkerConfig(&worker_config, proc_config.conf_file);

  // Setup runtime logging and business logging.'
  // TODO(YUYUE):

  // Connect an channel to communicate with master process

  // Addon.LoadingAddon(path);
  // EventLoop loop;
  // Client client;

  // client.SyncRead(cpulayout);
  // threads.Running();
  // client.AsyncRead(
  //   // On update task, update the data
  // );

  // Initialize runtime environment, e.g. channel.

  // When channel has established, acqurie the remaining CPU information
  // and ues for allocate the worker threads, eventually, respond a message
  // to master process indicated that is done.

  while (true) {
    sleep(2);
  }
}

}  // namespace oak
