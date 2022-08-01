// Copyright 2022 The Oak Authors.

#include "oak/config.h"

#include <assert.h>
#include <fstream>
#include <unordered_map>

#include "json/json.h"
#include "oak/logging/logging.h"

namespace oak {
namespace {

using ModuleParser = bool (*)(const char* node, const Json2::Value&, Module*);

bool KafkaModuleParser(const char* node,
                       const Json2::Value& root,
                       Module* module) {
  module->type = Module::Type::KAFKA;

  if (root.isMember("enable"))
    module->enable = root["enable"].asBool();
  if (!module->enable) {
    OAK_INFO("%s.module.kafka.enable is not enabled.\n", node);
    return false;
  }

  KafkaConfig config;

  if (!root.isMember("bootstrap")) {
    OAK_INFO("%s.module.kafka.bootstrap is not found.\n", node);
    return false;
  }
  config.bootstrap = root["bootstrap"].asString();

  if (!root.isMember("topic")) {
    OAK_INFO("%s.module.kafka.topic is not found.\n", node);
    return false;
  }
  config.topic = root["topic"].asString();

  if (root.isMember("group"))
    config.group = root["group"].asBool();

  if (config.group) {
    config.group_name.append("consumer_group_");
    config.group_name.append(config.topic);
  }

  module->config = std::move(config);
  return true;
}

bool StreamPipeModuleParser(const char* node,
                            const Json2::Value& root,
                            Module* module) {
  module->type = Module::Type::STREAMPIPE;

  if (root.isMember("enable"))
    module->enable = root["enable"].asBool();
  if (!module->enable) {
    OAK_INFO("%s.module.streampipe.enable is not enabled.\n", node);
    return false;
  }

  StreamPipeConfig config;

  if (!root.isMember("address")) {
    OAK_INFO("%s.module.streampipe.address is not found.\n", node);
    return false;
  }
  config.address = root["address"].asString();

  module->config = std::move(config);
  return 0;
}

const std::unordered_map<std::string, ModuleParser> kModuleParsers{
  {"kafka",         KafkaModuleParser},
  {"streampipe",    StreamPipeModuleParser}
};

void ParseModule(const char* node,
                 const Json2::Value& root,
                 std::vector<Module>* modules) {
  assert(root.isObject() && "Invalid module config");

  for (auto const& module_name : root.getMemberNames()) {
    auto it = kModuleParsers.find(module_name);
    if (it == kModuleParsers.cend()) {
      OAK_ERROR("Unknown module \'%s.module.%s\' has been ignored.\n",
          node, module_name);
      continue;
    }
    Module module;
    if (it->second(node, root[module_name], &module))
      modules->push_back(std::move(module));
  }
}
}  // anonymous namespace

Config::Config(const char* fname) {
  std::ifstream in(fname);
  if (!in) return;

  auto features = Json2::Features::strictMode();
  Json2::Reader reader(features);
  Json2::Value root;

  if (!reader.parse(in, root, false)) {
    OAK_FATAL("Read %s failed: %s\n",
        fname, reader.getFormattedErrorMessages().c_str());
  }

  if (root.isMember("task")) {
    auto& task = root["task"];
    if (task.isMember("module")) {
      ParseModule("task", task["module"], &task_config.modules);
    }
  }

  if (root.isMember("source")) {
    auto& src = root["source"];
    if (src.isMember("module")) {
      ParseModule("", src["module"], &source_config.modules);
    }
  }

  if (root.isMember("parse")) {
    auto& parse = root["parse"];
    if (parse.isMember("module")) {
      ParseModule("parse", parse["module"], &parse_config.modules);
    }
  }

  if (root.isMember("sink")) {
    auto& sink = root["task"];
    if (sink.isMember("module")) {
      ParseModule("sink", sink["module"], &sink_config.modules);
    }
  }
}

}  // namespace oak
