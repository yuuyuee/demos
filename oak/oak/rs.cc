// Copyright 2022 The Oak Authors.

#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <signal.h>
#include <string>

#include "oak/addons/public/compiler.h"
#include "oak/addons/public/platform.h"
#include "oak/addons/public/version.h"
#include "oak/addons/module.h"
#include "oak/common/macros.h"
#include "oak/common/format.h"
#include "oak/common/fs.h"
#include "oak/common/debug.h"
#include "oak/common/system.h"
#include "oak/common/throw_delegate.h"
#include "oak/logging/logging.h"

#include "oak/config.h"
#include "oak/addons/proxy.h"

namespace {

#define OAK_LINE_STR(x) x "\n"
#define OAK_RS_USAGE                              \
  OAK_LINE_STR("Usage: rs <-h>")                  \
  OAK_LINE_STR("Version: " OAK_VERSION_STR)       \
  OAK_LINE_STR("Options:")                        \
  OAK_LINE_STR("    -h print help")

int stop_process = 0;
void StopProcess(int signo) {
  assert(signo == SIGINT || signo == SIGQUIT || signo == SIGTERM);
  stop_process = 1;
}

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

int GetFirstEnabledEventReceiver(
    oak::ModuleConfigDict const& modules,
    std::unique_ptr<oak::EventProxy>* event) {
  for (auto const& it : modules) {
    if (it.second.enable)
      return oak::EventProxy::Create(it.second.name, it.second.config, event);
  }
  return 0;
}

oak::ModuleConfig GetModuleConfig(const struct incoming_event& event) {
  oak::ModuleConfig module;
  std::string name = oak::Basename(event.file_name);
  auto pos = name.find('.');
  if (pos != std::string::npos)
    name = name.substr(0, pos);
  module.name = name;
  module.config["id"] = std::to_string(event.id);
  module.config["proto_type"] = std::to_string(event.proto_type);
  module.config["proto_name"] = std::string(event.proto_name);
  module.config["file_name"] = std::string(event.file_name);
  module.config["http_url"] = std::string(event.http_url);
  module.config["m_input_flow"] = std::to_string(event.m_input_flow);
  module.config["m_output_data"] = std::to_string(event.m_output_data);
  module.config["is_extract_netflow"] = std::to_string(event.is_extract_netflow);
  module.config["m_keep_flow"] = std::to_string(event.m_keep_flow);
  module.enable = true;
}

void RecoverOccurredEvent(
    oak::ModuleConfigDict* modules,
    const struct incoming_event* event, int len) {
  for (int i = 0; i < len; ++i) {
    oak::ModuleConfig module = GetModuleConfig(event[i]);
    for (auto const& it : module.config)
      (*modules)[module.name].config[it.first] = it.second;
    (*modules)[module.name].enable = module.enable;
  }
}

}  // anonymous namespace

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

  // Initialize events receiver.
  // Note: there is allowed to disable the event receiver, but it is
  // not allowed to fail to create event receiver if it is enabled.
  std::unique_ptr<oak::EventProxy> event;
  if (GetFirstEnabledEventReceiver(config.modules, &event) != 0) {
    OAK_ERROR("Create event receiver failed\n");
    return -1;
  }
  if (!event)
    OAK_WARNING("Not found configuration of the events receiver.\n");

  // Recover events that has occurred.
  int len = 0;
  struct incoming_event occurred_event[10];
  auto parser_moules = config.parser.modules;
  while ((len = event->Pull(occurred_event, OAK_ARRAYSIZE(occurred_event))) > 0)
    RecoverOccurredEvent(&config.parser.modules, occurred_event, len);
  if (len < 0) {
    OAK_ERROR("Pull out the occurred event failed\n");
    return -1;
  }

  // Creates worker thread.
  for (int i = 0; i < config.source.num_threads; ++i) {
    const oak::LogicCore* logic_core = oak::System::GetNextAvailLogicCore();
    if (logic_core == nullptr)
      oak::ThrowStdOutOfRange("No enough available CPU");
    std::string name = oak::Format("source-%d", i);
    oak::System::CreateThread(name, logic_core->mask,
                              []() { while (true) sleep(2); });
  }

  for (int i = 0; i < config.parser.num_threads; ++i) {
    const oak::LogicCore* logic_core = oak::System::GetNextAvailLogicCore();
    if (logic_core == nullptr)
      oak::ThrowStdOutOfRange("No enough available CPU");
    std::string name = oak::Format("parser-%d", i);
    oak::System::CreateThread(name, logic_core->mask,
                              []() { while (true) sleep(2); });
  }

  for (int i = 0; i < config.sink.num_threads; ++i) {
    const oak::LogicCore* logic_core = oak::System::GetNextAvailLogicCore();
    if (logic_core == nullptr)
      oak::ThrowStdOutOfRange("No enough available CPU");
    std::string name = oak::Format("sink-%d", i);
    oak::System::CreateThread(name, logic_core->mask,
                              []() { while (true) sleep(2); });
  }

  // Disable the failed the parser configuration and save it.
  oak::WriteConfig(config, proc_config.conf_file);

  // Setup master thread are guaranteed to be resident in current CPU.
  // int core = oak::System::GetCurrentCpu();
  // const oak::LogicCore* logic_core = oak::System::GetNextAvailLogicCore(core);
  // assert(logic_core != nullptr && "Logic error");
  // oak::System::SetThreadAffinity(pthread_self(), logic_core->mask);
  // oak::System::ThreadYield();

  // Waiting for task event of the outside.
  signal(SIGQUIT, StopProcess);
  signal(SIGINT, StopProcess);
  signal(SIGTERM, StopProcess);

  while (stop_process == 0) {
    sleep(2);
  }

  return 0;
}

