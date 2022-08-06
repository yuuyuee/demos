// Copyright 2022 The Oak Authors.

#ifndef OAK_CONFIG_H_
#define OAK_CONFIG_H_

#include <vector>
#include <string>
#include <memory>
#include <unordered_map>

namespace oak {

// ProcessConfig
//
// Processes configuration is not immutable.

struct ProcessConfig {
  std::string proc_name;      ///< process name
  std::string home;           ///< process home directory
  std::string bin_dir;        ///< process binary directory
  std::string etc_dir;        ///< process configuration directory
  std::string log_dir;        ///< process log directory
  std::string addons_dir;     ///< process addons directory

  std::string cmd_channel;    ///< command message report
  std::string event_channel;  ///< event message report
  std::string crash_channel;  ///< worker crash message report

  std::string pid_file;       ///< absolute path of the process pid file
  std::string log_file;       ///< absolute path of the process log file
  std::string conf_file;      ///< absolute path of the process config file
};

// Built-in master process configuration.
const ProcessConfig& GetMasterProcessConfig();

// Built-in worker process configuration.
const ProcessConfig& GetWorkerProcessConfig();

// ModuleConfig

struct ModuleConfig {
  std::string name;     ///< module name
  bool enable{false};   ///< whether module is enable
  std::unordered_map<std::string, std::string> param;
                        ///< module parameters
};

// MasterConfig

struct MasterConfig {
  std::string log_method;
  std::vector<ModuleConfig> modules;
};

void ReadMasterConfig(MasterConfig* config, const std::string& fname);
void WriteMasterConfig(const MasterConfig& config, const std::string& fname);

// WorkerConfig

struct SourceConfig {
  int num_threads{-1};
  std::vector<ModuleConfig> modules;
};

struct ParserConfig {
  int num_threads{-1};
  std::vector<ModuleConfig> modules;
};

struct SinkConfig {
  int num_threads{-1};
  std::vector<ModuleConfig> modules;
};

struct WorkerConfig {
  std::string name;
  std::string role;
  std::string log_method;
  SourceConfig source;
  ParserConfig parser;
  SinkConfig sink;
};

}  // namespace oak

#endif  // OAK_CONFIG_H_
