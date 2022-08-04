// Copyright 2022 The Oak Authors.

#ifndef OAK_CONFIG_H_
#define OAK_CONFIG_H_

#ifndef _GUN_SOURCE
# define _GUN_SOURCE
#endif

#include <sched.h>
#include <vector>
#include <string>
#include <unordered_map>

namespace oak {
struct ModuleConfig {
  // module name, e.g. kafka, streampipe
  std::string module_name;

  // Checks module is enable
  bool enable{false};

  // module address, e.g. kafka bootstrap, streampipe address
  std::string address;

  // module parameter
  std::unordered_map<std::string, std::string> parameter;
};

#define OAK_RUNTIME_DIRECTORY "/var/run/oak"
#define OAK_TASK_CHANNEL      (OAK_RUNTIME_DIRECTORY "/.oak_task_channel")
#define OAK_CRASH_CHANNEL     (OAK_RUNTIME_DIRECTORY "/.oak_crash_channel")

struct MasterConfig {
  std::string process_name;
  std::string log_method;
  std::string runtime_directory;
  std::string task_channel;
  std::string crash_channel;
  std::vector<ModuleConfig> task_module;
  cpu_set_t available_cpu_set;
};

struct WorkerConfig {
  std::string process_name;
  std::string log_method;
  std::string runtime_directory;
  std::string task_channel;
  std::string crash_channel;
  std::vector<ModuleConfig> source_module;
  std::vector<ModuleConfig> sink_module;
  cpu_set_t available_cpu_set;
};

}  // namespace oak

#endif  // OAK_CONFIG_H_
