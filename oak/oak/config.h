// Copyright 2022 The Oak Authors.

#ifndef OAK_CONFIG_H_
#define OAK_CONFIG_H_

#include <vector>
#include <string>
#include <unordered_map>

namespace oak {

// ProcessConfig

#define OAK_ETC_DIR       "etc"
#define OAK_LOG_DIR       "log"
#define OAK_ADDONS_DIR    "addons"
#define OAK_TASK_CHANNEL  ".oak_task_channel"
#define OAK_CRASH_CHANNEL ".oak_crash_channel"
#define OAK_CONFIG_FILE   "setup.json"

struct ProcessConfig {
  std::string home;
  std::string bin_dir;
  std::string etc_dir;
  std::string log_dir;
  std::string addons_dir;
  std::string task_channel;
  std::string crash_channel;
  std::string config_file;
};

void InitProcessConfig(ProcessConfig* config);

#define OAK_MASTER_NAME     "OAK Master"
#define OAK_MASTER_ROLE     "master"
#define OAK_MASTER_PIDNAME  (OAK_MASTER_ROLE ".pid")
#define OAK_MASTER_LOGNAME  (OAK_MASTER_ROLE ".log")

#define OAK_WORKER_NAME     "OAK Worker"
#define OAK_WORKER_ROLE     "worker"
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

// MasterConfig

struct MasterConfig {
  std::string name;
  std::string role;
  std::string pid_name;
  std::string log_name;
  std::string log_method;
  std::vector<ModuleConfig> task_modules;
};

void InitMasterConfig(MasterConfig* config, const std::string& fname);

// WorkerConfig

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
  std::string name;
  std::string role;
  std::string pid_name;
  std::string log_name;
  std::string log_method;
  SourceConfig source;
  ParserConfig parser;
  SinkConfig sink;
};

void InitWorkerConfig(WorkerConfig* config, const std::string& fname);

}  // namespace oak

#endif  // OAK_CONFIG_H_
