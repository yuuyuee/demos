// Copyright 2022 The Oak Authors.

#ifndef OAK_CONFIG_H_
#define OAK_CONFIG_H_

#include <vector>
#include <string>

#include "oak/common/any.h"

namespace oak {
struct KafkaConfig {
  std::string bootstrap;
  std::string topic;
  bool group;
  std::string group_name;

  KafkaConfig(): bootstrap(), topic(), group(false), group_name() {}
};

struct StreamPipeConfig {
  std::string address;

  StreamPipeConfig(): address() {}
};

struct Module {
  enum Type {
    KAFKA,
    STREAMPIPE,
    UNKNOWN
  };

  bool enable;
  enum Type type;
  Any config;

  Module(): enable(false), type(Type::UNKNOWN), config() {}
};

struct TaskConfig {
  std::vector<Module> modules;
};

struct SourceConfig {
  int num_threads;
  std::vector<Module> modules;
};

struct ParseConfig {
  int num_threads;
  std::vector<Module> modules;
};

struct SinkConfig {
  int num_threads;
  std::vector<Module> modules;
};

struct Config {
  TaskConfig task_config;
  SourceConfig source_config;
  ParseConfig parse_config;
  SinkConfig sink_config;

  explicit Config(const char* fname);
};

}  // namespace oak

#endif  // OAK_CONFIG_H_
