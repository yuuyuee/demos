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
#define OAK_TASK_CHANNEL    ".oak_task_channel"
#define OAK_CRASH_CHANNEL   ".oak_crash_channel"
#define OAK_CONFIG_NAME     "etc/setup.json"
#define OAK_LOG_DIR         "log"
#define OAK_ADDONS_DIR      "addons"

#define OAK_MASTER_NAME     "OAK Master"
#define OAK_MASTER_ROLE     "master"
#define OAK_MASTER_PIDNAME  (OAK_MASTER_ROLE ".pid")
#define OAK_MASTER_LOGNAME  (OAK_MASTER_ROLE ".log")

#define OAK_WORKER_NAME     "OAK Worker"
#define OAK_WORKER_ROLE     "master"
#define OAK_WORKER_PIDNAME  (OAK_WORKER_ROLE ".pid")
#define OAK_WORKER_LOGNAME  (OAK_WORKER_ROLE ".log")

struct ModuleConfig {
  // module name, e.g. kafka, streampipe
  std::string name;

  // Checks module is enable
  bool enable{false};

  // module parameter
  std::unordered_map<std::string, std::string> param;
};

struct ProcessConfig {
  std::string name;
  std::string role;
  std::string home;
  std::string pid_name;
  std::string log_name;
  cpu_set_t available_cpu_set;
};

struct MasterConfig {
  ProcessConfig process;
  std::string log_method;
  std::string task_channel;
  std::string crash_channel;
  std::vector<ModuleConfig> task_modules;
};

void LoadMasterConfig(MasterConfig* config, const char* fname);

struct SourceConfig {
  int num_threads;
  std::vector<ModuleConfig> modules;
};

struct ParserConfig {
  int num_threads;
  std::vector<ModuleConfig> modules;
};

struct SinkConfig {
  int num_threads;
  std::vector<ModuleConfig> modules;
};

struct WorkerConfig {
  ProcessConfig process;
  std::string log_method;
  std::string task_channel;
  std::string crash_channel;

  SourceConfig source;
  ParserConfig parser;
  SinkConfig sink;
};

void LoadWorkerConfig(WorkerConfig* config, const char* fname);

}  // namespace oak

#endif  // OAK_CONFIG_H_
