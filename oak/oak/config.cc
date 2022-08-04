// Copyright 2022 The Oak Authors.

#include "oak/config.h"

#include <assert.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <utility>

#include "json/json.h"
#include "oak/logging/logging.h"

namespace oak {
namespace {

void LoadModuleConfig(const char* own,
                      const Json2::Value& node,
                      std::vector<ModuleConfig>* modules) {
  assert(node.isObject() && "Invalid module config");

  for (const auto& name : node.getMemberNames()) {
    ModuleConfig module;
    module.name = name;
    module.enable = false;

    const auto& module_node = node[name];
    for (const auto& key : module_node.getMemberNames()) {
      if (key == "enable") {
        module.enable = module_node[key].asBool();
      } else {
        module.param[key] = module_node[key].asString();
      }
    }

    if (!module.enable) {
      OAK_WARNING("%s.modules.%s will not be enabled.\n", own, name.c_str());
      continue;
    }

    modules->push_back(std::move(module));
  }
}

}  // anonymous namespace

void LoadMasterConfig(MasterConfig* config, const char* fname) {
  std::ifstream in(fname);
  if (!in)
    OAK_FATAL("Read %s failed\n", fname);

  config->process.name = OAK_MASTER_NAME;
  config->process.role = OAK_MASTER_ROLE;
  char home[PATH_MAX];
  char* ptr = realpath(".", home);
  if (!ptr)
    OAK_FATAL("Unable to get home directory: %s\n", strerror(errno));
  config->process.home = home;
  memset(&config->process.available_cpu_set,
         0,
         sizeof(config->process.available_cpu_set));
  config->process.pid_name = OAK_MASTER_PIDNAME;
  config->process.log_name = OAK_MASTER_LOGNAME;

  config->task_channel = OAK_TASK_CHANNEL;
  config->crash_channel = OAK_CRASH_CHANNEL;

  auto features = Json2::Features::strictMode();
  Json2::Reader reader(features);
  Json2::Value node;

  if (!reader.parse(in, node, false)) {
    OAK_FATAL("Read %s failed: %s\n",
        fname, reader.getFormattedErrorMessages().c_str());
  }

  if (!node.isMember("master"))
    OAK_FATAL("Read %s failed: not found node \'master\'\n", fname);

  const auto& master = node["master"];

  if (master.isMember("log_method"))
    config->log_method = master["log_method"].asString();

  if (master.isMember("modules"))
    LoadModuleConfig("master", master["modules"], &config->task_modules);
}

void LoadWorkerConfig(WorkerConfig* config, const char* fname) {
  std::ifstream in(fname);
  if (!in)
    OAK_FATAL("Read %s failed\n", fname);

  config->process.name = OAK_WORKER_NAME;
  config->process.role = OAK_WORKER_ROLE;
  char home[PATH_MAX];
  char* ptr = realpath("..", home);
  if (!ptr)
    OAK_FATAL("Unable to get home directory: %s\n", strerror(errno));
  config->process.home = home;
  config->process.pid_name = OAK_WORKER_PIDNAME;
  config->process.log_name =  OAK_WORKER_LOGNAME;
  memset(&config->process.available_cpu_set,
         0,
         sizeof(config->process.available_cpu_set));

  config->task_channel = OAK_TASK_CHANNEL;
  config->crash_channel = OAK_CRASH_CHANNEL;

  auto features = Json2::Features::strictMode();
  Json2::Reader reader(features);
  Json2::Value node;

  if (!reader.parse(in, node, false)) {
    OAK_FATAL("Read %s failed: %s\n",
        fname, reader.getFormattedErrorMessages().c_str());
  }

  if (!node.isMember("worker"))
    OAK_FATAL("Read %s failed: not found node \'worker\'\n", fname);

  const auto& worker = node["worker"];

  if (worker.isMember("log_method"))
    config->log_method = worker["log_method"].asString();

  if (worker.isMember("source")) {
    const auto& source = worker["source"];

    if (source.isMember("num_threads"))
      config->source.num_threads = source["num_threads"].asUInt();

    if (source.isMember("modules"))
      LoadModuleConfig("source", source["modules"], &config->source.modules);
  }

  if (worker.isMember("parser")) {
    const auto& parser = worker["parser"];

    if (parser.isMember("num_threads"))
      config->parser.num_threads = parser["num_threads"].asUInt();

    if (parser.isMember("modules"))
      LoadModuleConfig("parser", parser["modules"], &config->parser.modules);
  }

  if (worker.isMember("sink")) {
    const auto& sink = worker["sink"];

    if (sink.isMember("num_threads"))
      config->sink.num_threads = sink["num_threads"].asUInt();

    if (sink.isMember("modules"))
      LoadModuleConfig("sink", sink["modules"], &config->sink.modules);
  }
}

}  // namespace oak
