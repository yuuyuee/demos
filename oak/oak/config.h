// Copyright 2022 The Oak Authors.

#ifndef OAK_CONFIG_H_
#define OAK_CONFIG_H_

#include <vector>
#include <string>

#include "oak/common/any.h"

namespace oak {
struct KafkaConfig {
  // Kafka bootstrap
  std::string bootstrap;

  // Kafka topic
  std::string topic;

  // If this config is used for consumers, indicates whether
  // it belongs to the enabled consumer group
  bool group;

  // Consumer goup name when group is true
  std::string group_name;

  KafkaConfig(): bootstrap(), topic(), group(false), group_name() {}
};

struct StreamPipeConfig {
  // StreamPipe address and port
  std::string address;

  StreamPipeConfig(): address() {}
};

struct Module {
  enum Type {
    KAFKA,
    STREAMPIPE,
    UNKNOWN
  };

  // Whether the module is enable
  bool enable;

  // module type
  enum Type type;

  // module config
  Any config;

  Module(): enable(false), type(Type::UNKNOWN), config() {}
};

struct TaskConfig {
  std::vector<Module> modules;
};

struct SourceConfig {
  // The number of the threads of the source module
  int num_threads;
  std::vector<Module> modules;
};

struct ParseConfig {
  // The number of the threads of the parser module
  int num_threads;
  std::vector<Module> modules;
};

struct SinkConfig {
  // The number of the threads of the sink module
  int num_threads;
  std::vector<Module> modules;
};

struct WorkerConfig {
  std::string log_method;
  SourceConfig source_config;
  ParseConfig parse_config;
  SinkConfig sink_config;
};

struct Config {
  std::string log_method;
  TaskConfig task_config;
  WorkerConfig worker_config;

  explicit Config(const char* fname);
};

}  // namespace oak

#endif  // OAK_CONFIG_H_
