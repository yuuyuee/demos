// Copyright 2022 The Oak Authors.

#include "oak/config.h"

#include <assert.h>
#include <fstream>
#include <utility>
#include <unordered_map>

#include "json/json.h"
#include "oak/logging/logging.h"

namespace oak {
namespace {

void ParseModule(const char* node,
                 const Json2::Value& root,
                 std::vector<Module>* modules) {
  assert(root.isObject() && "Invalid module config");

  for (const auto& module_name : root.getMemberNames()) {
    auto it = kModuleParsers.find(module_name);
    if (it == kModuleParsers.cend()) {
      OAK_ERROR("Unknown module \'%s.%s\' has been ignored.\n",
          node, module_name.c_str());
      continue;
    }
    Module module;
    if (it->second(node, root[module_name], &module)) {
      assert(module.enable && module.type != Module::Type::UNKNOWN &&
          "Invalid module config");
      modules->push_back(std::move(module));
    }
  }
}

void ParseSourceConfig(const Json2::Value& root, SourceConfig* config) {
  config->num_threads = -1;
  if (root.isMember("num_threads"))
    config->num_threads = root["num_threads"].asUInt();

  if (root.isMember("module"))
    ParseModule("worker.source.module", root["module"], &config->modules);
}

void ParseParserConfig(const Json2::Value& root, SourceConfig* config) {
  config->num_threads = -1;
  if (root.isMember("num_threads"))
    config->num_threads = root["num_threads"].asUInt();

  if (root.isMember("module"))
    ParseModule("worker.parser.module", root["module"], &config->modules);
}

void ParseSinkConfig(const Json2::Value& root, SourceConfig* config) {
  config->num_threads = -1;
  if (root.isMember("num_threads"))
    config->num_threads = root["num_threads"].asUInt();

  if (root.isMember("module"))
    ParseModule("worker.sink.module", root["module"], &config->modules);
}

void ParseWorkerConfig(const Json2::Value& root, WorkerConfig* config) {
  if (root.isMember("source"))
    ParseSourceConfig(root["source"], &config->source_config);

  if (root.isMember("parser"))
    ParseParserConfig(root["parser"], &config->source_config);

  if (root.isMember("sink"))
    ParseSinkConfig(root["sink"], &config->source_config);
}
}  // anonymous namespace

void InitMasterConfig(const char* fname, MasterConfig* config) {
  std::ifstream in(fname);
  if (!in)
    OAK_FATAL("Read %s failed\n", fname);

  auto features = Json2::Features::strictMode();
  Json2::Reader reader(features);
  Json2::Value root;

  if (!reader.parse(in, root, false)) {
    OAK_FATAL("Read %s failed: %s\n",
        fname, reader.getFormattedErrorMessages().c_str());
  }

  if (root.isMember("log_method"))
    config->log_method = root["log_method"].asString();

  if (!root.isMember("master"))
    OAK_FATAL("Read %s failed: not found node \'master\'\n", fname);

  const auto& master = root["master"];

  if (master.isMember("log_method"))
    config->log_method = root["log_method"].asString();

  if (master.isMember("kafka"))
    ParseWorkerConfig(root["worker"], &config->worker_config);
}

void InitWorkerConfig(const char* fname, WorkerConfig* config) {
  std::ifstream in(fname);
  if (!in) return;

  auto features = Json2::Features::strictMode();
  Json2::Reader reader(features);
  Json2::Value root;

  if (!reader.parse(in, root, false)) {
    OAK_FATAL("Read %s failed: %s\n",
        fname, reader.getFormattedErrorMessages().c_str());
  }

  if (root.isMember("log_method"))
    config->log_method = root["log_method"].asString();

  if (root.isMember("task")) {
    const auto& node = root["task"];
    if (node.isMember("module"))
      ParseModule("task", node["module"], &config->task_config.modules);
  }

  if (root.isMember("worker"))
    ParseWorkerConfig(root["worker"], &config->worker_config);
}

}  // namespace oak
