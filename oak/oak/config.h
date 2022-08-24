// Copyright 2022 The Oak Authors.

#ifndef OAK_CONFIG_H_
#define OAK_CONFIG_H_

#include <string>
#include <memory>
#include <unordered_map>

namespace oak {

using Dict = std::unordered_map<std::string, std::string>;

// ProcessConfig
//
// Processes configuration is immutable.

struct ProcessConfig {
  std::string proc_name;    ///< process name
  std::string home;         ///< process home directory
  std::string bin_dir;      ///< process binary directory
  std::string etc_dir;      ///< process configuration directory
  std::string log_dir;      ///< process log directory
  std::string addons_dir;   ///< process addons directory
  std::string guard_file;   ///< absolute path of the process pid file
  std::string log_file;     ///< absolute path of the process log file
  std::string conf_file;    ///< absolute path of the process config file
  std::string crash_file;   ///< absolute path of the both process crash file
};

// Built-in master process configuration.
const ProcessConfig& GetProcessConfig();

// ModuleConfig

struct ModuleConfig {
  int id{-1};           ///< module ID
  std::string name;     ///< module name
  bool enable{false};   ///< whether or not the module is enable
  std::string path;     ///< module path
  Dict config;          ///< module config
};

using ModuleConfigDict = std::unordered_map<std::string, ModuleConfig>;

// Config

struct SourceConfig {
  std::string comment;
  int num_threads{-1};
  ModuleConfigDict modules;
};

struct ParserConfig {
  std::string comment;
  int num_threads{-1};
  ModuleConfigDict modules;
};

struct SinkConfig {
  std::string comment;
  int num_threads{-1};
  ModuleConfigDict modules;
};

struct Config {
  std::string comment;
  std::string log_method;
  ModuleConfigDict modules;
  SourceConfig source;
  ParserConfig parser;
  SinkConfig sink;
};

void ReadConfig(Config* config, const std::string& fname);
void WriteConfig(const Config& config, const std::string& fname);

}  // namespace oak

#endif  // OAK_CONFIG_H_
