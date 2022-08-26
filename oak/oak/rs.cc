// Copyright 2022 The Oak Authors.

#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <signal.h>
#include <string>

#include "oak/addons/public/compiler.h"
#include "oak/addons/public/platform.h"
#include "oak/addons/public/version.h"
#include "oak/addons/public/event.h"
#include "oak/addons/event_handle.h"
#include "oak/common/macros.h"
#include "oak/common/format.h"
#include "oak/common/fs.h"
#include "oak/common/debug.h"
#include "oak/common/system.h"
#include "oak/common/stringpiece.h"
#include "oak/common/throw_delegate.h"
#include "oak/logging/logging.h"
#include "oak/config.h"

using std::string;
using std::unique_ptr;

using namespace oak;  // NOLINT

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

bool CreateGuardFile(const string& guard_file) {
  string pid = Format("%d", getpid());
  File file = File::MakeWritableFile(guard_file);
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
    ModuleConfigDict const& modules, unique_ptr<EventHandle>* handle) {
  for (auto const& it : modules) {
    if (it.second.enable)
      return EventHandleFactory(it.second, handle);
  }
  return 0;
}

ModuleConfig EventToParserModuleConfig(const string& addon_dir,
                                       const struct incoming_event& event) {
  assert(event.type == ET_PARSER && "Invalid event.type");
  ModuleConfig module;

  // module.id = std::to_string(event.id)
  for (size_t i = 0; i < event.args.size; ++i) {
    const struct oak_dict_entry* kv = &event.args.elems[i];
    string key(static_cast<const char*>(kv->key.ptr), kv->key.size);
    string value(static_cast<const char*>(kv->value.ptr), kv->value.size);
    if (key.empty() || value.empty())
      continue;
    if (key == OAK_PROTOCOL_TYPE) {
      module.id = std::stoi(value);
    } else if (key == OAK_FILE_NAME) {
      string file_name = Basename(value);
      module.path = addon_dir + "/" + file_name;

      string module_name = file_name;
      auto pos = module_name.rfind('.');
      if (pos != string::npos)
        module_name = module_name.substr(0, pos);
      module.name = module_name;
    }
    module.config[key] = value;
  }

  module.enable = true;
  return module;
}

void RetrievalEvent(const string& addon_dir,
                    ModuleConfigDict* modules,
                    const struct incoming_event* event,
                    int len) {
  for (int i = 0; i < len; ++i) {
    ModuleConfig module = EventToParserModuleConfig(addon_dir, event[i]);
    for (auto const& it : module.config)
      (*modules)[module.name].config[it.first] = it.second;
    (*modules)[module.name].enable = module.enable;
    (*modules)[module.name].id = module.id;
    (*modules)[module.name].path = module.path;
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
  SetSignalAltStack();
  RegisterFailureSignalHandler();
  RegisterFailureMessageHandler(STDERR_FILENO);

  // Changes working directory.
  string path = DirectoryName(argv[0]);
  ChangeWorkDirectory(path);

  // Initialize process configuration.
  const ProcessConfig& proc_config = GetProcessConfig();

  // Setup crash handler.
  CreateDirectory(proc_config.log_dir);
  // RegisterFailureMessageHandler(proc_config.crash_file);

  // Locks pid file to checking whther process is running.
  CreateDirectory(DirectoryName(proc_config.guard_file));
  if (!CreateGuardFile(proc_config.guard_file)) {
    // Other process has already locked.
    OAK_ERROR("%s: Process is already running.\n",
              proc_config.proc_name.c_str());
    return -1;
  }

  OAK_INFO("%s: Change working directory to %s\n",
           proc_config.proc_name.c_str(),
           GetCurrentDirectory().c_str());
  assert(proc_config.bin_dir == GetCurrentDirectory());

  Config config;
  ReadConfig(&config, proc_config.conf_file);

  // Setup logger.
  // TODO(YUYUE):

  // Initialize events receiver.
  // Note: there is allowed to disable the event receiver, but it is
  // not allowed to fail to create event receiver if it is enabled.
  unique_ptr<EventHandle> event_handle;
  if (GetFirstEnabledEventReceiver(config.modules, &event_handle) != 0) {
    OAK_ERROR("Create event receiver failed\n");
    return -1;
  }
  if (!event_handle)
    OAK_WARNING("Not found configuration of the events receiver.\n");

  // Recover events that has occurred.
  const int occurred_event_size = 10;
  int len = 0;
  struct incoming_event occurred_event[10];
  auto parser_moules = config.parser.modules;
  while ((len = event_handle->Pull(occurred_event, occurred_event_size)) > 0) {
    RetrievalEvent(proc_config.addons_dir, &parser_moules, occurred_event, len);
  }
  if (len < 0) {
    OAK_ERROR("Pull out the occurred event failed\n");
    return -1;
  }

  // Creates worker thread.


  // Disable the failed the parser configuration and save it.
  WriteConfig(config, proc_config.conf_file);

  // Setup master thread are guaranteed to be resident in current CPU.
  // int core = System::GetCurrentCpu();
  // const LogicCore* logic_core = System::GetNextAvailLogicCore(core);
  // assert(logic_core != nullptr && "Logic error");
  // System::SetThreadAffinity(pthread_self(), logic_core->mask);
  // System::ThreadYield();

  // Waiting for task event of the outside.
  signal(SIGQUIT, StopProcess);
  signal(SIGINT, StopProcess);
  signal(SIGTERM, StopProcess);

  while (stop_process == 0) {
    sleep(2);
  }

  return 0;
}

