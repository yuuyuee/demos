// Copyright 2022 The Oak Authors.

#include "oak/config.h"

#include <assert.h>
#include <fstream>
#include <utility>

#include "json/json.h"
#include "oak/logging/logging.h"
#include "oak/common/trivial.h"

namespace oak {
namespace {

void LoadModuleConfig(const std::string& own,
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
      OAK_WARNING("%s.modules.%s will not be enabled.\n",
                  own.c_str(), name.c_str());
      continue;
    }

    modules->push_back(std::move(module));
  }
}

}  // anonymous namespace

void InitProcessConfig(ProcessConfig* config) {
  config->home          = GetRealPath("..");
  config->bin_dir       = GetRealPath(".");
  config->etc_dir       = config->home + "/" OAK_ETC_DIR;
  config->log_dir       = config->home + "/" OAK_LOG_DIR;
  config->addons_dir    = config->home + "/" OAK_ADDONS_DIR;
  config->task_channel  = config->bin_dir + "/" OAK_TASK_CHANNEL;
  config->crash_channel = config->bin_dir + "/" OAK_CRASH_CHANNEL;
  config->config_file   = config->etc_dir + "/" OAK_CONFIG_FILE;
}

void InitMasterConfig(MasterConfig* config, const std::string& fname) {
  std::ifstream in(fname.c_str());
  if (!in)
    OAK_FATAL("Read %s failed\n", fname.c_str());

  config->name = OAK_MASTER_NAME;
  config->role = OAK_MASTER_ROLE;
  config->pid_name = OAK_MASTER_PIDNAME;
  config->log_name = OAK_MASTER_LOGNAME;

  auto features = Json2::Features::strictMode();
  Json2::Reader reader(features);
  Json2::Value node;

  if (!reader.parse(in, node, false)) {
    OAK_FATAL("Read %s failed: %s\n",
              fname.c_str(),
              reader.getFormattedErrorMessages().c_str());
  }

  if (!node.isMember(config->role)) {
    OAK_FATAL("Read %s failed: not found role %s\n",
              fname.c_str(), config->role.c_str());
  }

  const auto& master = node[config->role];

  if (master.isMember("log_method"))
    config->log_method = master["log_method"].asString();

  if (master.isMember("modules")) {
    LoadModuleConfig(config->role,
                     master["modules"],
                     &config->task_modules);
  }
}

void InitWorkerConfig(WorkerConfig* config, const std::string& fname) {
  std::ifstream in(fname.c_str());
  if (!in)
    OAK_FATAL("Read %s failed\n", fname.c_str());

  config->name = OAK_WORKER_NAME;
  config->role = OAK_WORKER_ROLE;
  config->pid_name = OAK_WORKER_PIDNAME;
  config->log_name = OAK_WORKER_LOGNAME;

  auto features = Json2::Features::strictMode();
  Json2::Reader reader(features);
  Json2::Value node;

  if (!reader.parse(in, node, false)) {
    OAK_FATAL("Read %s failed: %s\n",
              fname.c_str(),
              reader.getFormattedErrorMessages().c_str());
  }

  if (!node.isMember(config->role)) {
    OAK_FATAL("Read %s failed: not found role %s\n",
              fname.c_str(), config->role.c_str());
  }

  const auto& worker = node[config->role];

  if (worker.isMember("log_method"))
    config->log_method = worker["log_method"].asString();

  if (worker.isMember("source")) {
    const auto& source = worker["source"];

    if (source.isMember("num_threads"))
      config->source.num_threads = source["num_threads"].asUInt();

    if (source.isMember("modules")) {
      LoadModuleConfig(config->role + ".source",
                       source["modules"],
                       &config->source.modules);
    }
  }

  if (worker.isMember("parser")) {
    const auto& parser = worker["parser"];

    if (parser.isMember("num_threads"))
      config->parser.num_threads = parser["num_threads"].asUInt();

    if (parser.isMember("modules")) {
      LoadModuleConfig(config->role + ".parser",
                       parser["modules"],
                       &config->parser.modules);
    }
  }

  if (worker.isMember("sink")) {
    const auto& sink = worker["sink"];

    if (sink.isMember("num_threads"))
      config->sink.num_threads = sink["num_threads"].asUInt();

    if (sink.isMember("modules")) {
      LoadModuleConfig(config->role + ".sink",
                       sink["modules"],
                       &config->sink.modules);
    }
  }
}

}  // namespace oak
