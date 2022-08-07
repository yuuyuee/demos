// Copyright 2022 The Oak Authors.

#include "oak/config.h"

#include <assert.h>
#include <fstream>

#include "json/json.h"
#include "oak/common/format.h"
#include "oak/common/throw_delegate.h"
#include "oak/common/fs.h"

namespace oak {
// ProcessConfig

#define OAK_ETC_DIR           "etc"
#define OAK_LOG_DIR           "log"
#define OAK_ADDONS_DIR        "addons"
#define OAK_CMD_CHANNEL       ".oak_cmd_channel"
#define OAK_EVENT_CHANNEL     ".oak_event_channel"
#define OAK_CRASH_CHANNEL     ".oak_crash_channel"
#define OAK_GUARD_FILE        "daemon.pid"

#define OAK_MASTER_PROC_NAME  "OAK Master"
#define OAK_MASTER_LOG_FILE   "master.log"
#define OAK_MASTER_CONF_FILE  "master.json"

#define OAK_WORKER_PROC_NAME  "OAK Worker"
#define OAK_WORKER_LOG_FILE   "worker.log"
#define OAK_WORKER_CONF_FILE  "worker.json"

const ProcessConfig& GetMasterProcessConfig() {
  static const ProcessConfig config = {
    .proc_name     = OAK_MASTER_PROC_NAME,
    .home          = GetRealPath(".."),
    .bin_dir       = GetCurrentDirectory(),
    .etc_dir       = config.home + "/" OAK_ETC_DIR,
    .log_dir       = config.home + "/" OAK_LOG_DIR,
    .addons_dir    = config.home + "/" OAK_ADDONS_DIR,
    .cmd_channel   = config.bin_dir + "/" OAK_CMD_CHANNEL,
    .event_channel = config.bin_dir + "/" OAK_EVENT_CHANNEL,
    .crash_channel = config.bin_dir + "/" OAK_CRASH_CHANNEL,
    .guard_file    = "/tmp/oak/" OAK_GUARD_FILE,
    .log_file      = config.bin_dir + "/" OAK_MASTER_LOG_FILE,
    .conf_file     = config.etc_dir + "/" OAK_MASTER_CONF_FILE,
  };
  return config;
}

const ProcessConfig& GetWorkerProcessConfig() {
  static const ProcessConfig config = {
    .proc_name     = OAK_WORKER_PROC_NAME,
    .home          = GetRealPath(".."),
    .bin_dir       = GetCurrentDirectory(),
    .etc_dir       = config.home + "/" OAK_ETC_DIR,
    .log_dir       = config.home + "/" OAK_LOG_DIR,
    .addons_dir    = config.home + "/" OAK_ADDONS_DIR,
    .cmd_channel   = config.bin_dir + "/" OAK_CMD_CHANNEL,
    .event_channel = config.bin_dir + "/" OAK_EVENT_CHANNEL,
    .crash_channel = config.bin_dir + "/" OAK_CRASH_CHANNEL,
    .guard_file      = "/tmp/oak/" OAK_GUARD_FILE,
    .log_file      = config.bin_dir + "/" OAK_WORKER_LOG_FILE,
    .conf_file     = config.etc_dir + "/" OAK_WORKER_CONF_FILE,
  };
  return config;
}

namespace {

void ReadModuleConfig(std::vector<ModuleConfig>* modules,
                      const Json2::Value& node) {
  for (const auto& name : node.getMemberNames()) {
    ModuleConfig module;
    module.name = name;

    const auto& module_node = node[name];
    for (const auto& key : module_node.getMemberNames()) {
      if (key == "enable") {
        module.enable = module_node[key].asBool();
      } else {
        module.param[key] = module_node[key].asString();
      }
    }

    modules->push_back(module);
  }
}

void WriteModuleConfig(const std::vector<ModuleConfig>& modules,
                       Json2::Value* node) {
  for (const auto& module : modules) {
    Json2::Value value(Json2::objectValue);
    value["enable"] = module.enable;
    for (const auto& kv : module.param)
      value[kv.first] = kv.second;
    (*node)[module.name] = value;
  }
}

}  // anonymous namespace

// MasterConfig

void ReadMasterConfig(MasterConfig* config, const std::string& fname) {
  File file = File::MakeReadOnlyFile(fname);
  std::string buffer;
  char tmp[1024];
  size_t n;
  while ((n = file.Read(tmp, 1024)) > 0)
    buffer.append(tmp, n);

  auto features = Json2::Features::strictMode();
  Json2::Reader reader(features);
  Json2::Value node;

  if (!reader.parse(buffer, node, false)) {
    ThrowStdInvalidArgument(
        Format("Parse json \'%s\' failed: %s\n",
               fname.c_str(),
               reader.getFormattedErrorMessages().c_str()));
  }

  if (node.isMember("comment"))
    config->comment = node["comment"].asString();

  if (node.isMember("log_method"))
    config->log_method = node["log_method"].asString();

  if (node.isMember("modules")) {
    ReadModuleConfig(&config->modules, node["modules"]);
  }
}

void WriteMasterConfig(const MasterConfig& config, const std::string& fname) {
  Json2::Value node(Json2::objectValue);
  node["log_method"] = config.log_method;
  node["modules"] = Json2::Value(Json2::objectValue);
  WriteModuleConfig(config.modules, &(node["modules"]));

  Json2::StyledWriter writer;
  std::string buffer = writer.write(node);
  File::MakeWritableFile(fname).Write(buffer);
}

void ReadWorkerConfig(WorkerConfig* config, const std::string& fname) {
  File file = File::MakeReadOnlyFile(fname);
  std::string buffer;
  char tmp[1024];
  size_t n;
  while ((n = file.Read(tmp, 1024)) > 0)
    buffer.append(tmp, n);

  auto features = Json2::Features::strictMode();
  Json2::Reader reader(features);
  Json2::Value node;

  if (!reader.parse(buffer, node, false)) {
    ThrowStdInvalidArgument(
        Format("Parse json \'%s\' failed: %s\n",
               fname.c_str(),
               reader.getFormattedErrorMessages().c_str()));
  }

  if (node.isMember("comment"))
    config->comment = node["comment"].asString();

  if (node.isMember("log_method"))
    config->log_method = node["log_method"].asString();

  // Read source configuration
  if (node.isMember("source")) {
    const auto& source = node["source"];

    if (source.isMember("comment"))
      config->source.comment = source["comment"].asString();

    if (source.isMember("num_threads"))
      config->source.num_threads = source["num_threads"].asUInt();

    if (source.isMember("modules")) {
      ReadModuleConfig(&config->source.modules, source["modules"]);
    }
  }

  // Read parser configuration
  if (node.isMember("parser")) {
    const auto& parser = node["parser"];

    if (parser.isMember("parser"))
      config->parser.comment = parser["comment"].asString();

    if (parser.isMember("num_threads"))
      config->parser.num_threads = parser["num_threads"].asUInt();

    if (parser.isMember("modules")) {
      ReadModuleConfig(&config->parser.modules, parser["modules"]);
    }
  }

  // Read sink configuration
  if (node.isMember("sink")) {
    const auto& sink = node["sink"];

    if (sink.isMember("comment"))
      config->sink.comment = sink["comment"].asString();

    if (sink.isMember("num_threads"))
      config->sink.num_threads = sink["num_threads"].asUInt();

    if (sink.isMember("modules")) {
      ReadModuleConfig(&config->sink.modules, sink["modules"]);
    }
  }
}

void WriteWorkerConfig(const WorkerConfig& config, const std::string& fname) {
  Json2::Value node(Json2::objectValue);
  node["comment"] = config.comment;
  node["log_method"] = config.log_method;

  // write source configuration
  node["source"] = Json2::Value(Json2::objectValue);
  auto& source = node["source"];
  source["comment"] = config.source.comment;
  source["num_threads"] = config.source.num_threads;
  WriteModuleConfig(config.source.modules, &(source["modules"]));

  // write parser configuration
  node["parser"] = Json2::Value(Json2::objectValue);
  auto& parser = node["parser"];
  parser["comment"] = config.parser.comment;
  parser["num_threads"] = config.parser.num_threads;
  WriteModuleConfig(config.parser.modules, &(parser["modules"]));

  // write sink configuration
  node["sink"] = Json2::Value(Json2::objectValue);
  auto& sink = node["sink"];
  sink["comment"] = config.sink.comment;
  sink["num_threads"] = config.sink.num_threads;
  WriteModuleConfig(config.sink.modules, &(sink["modules"]));

  Json2::StyledWriter writer;
  std::string buffer = writer.write(node);
  File::MakeWritableFile(fname).Write(buffer);
}

}  // namespace oak
