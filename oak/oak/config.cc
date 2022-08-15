// Copyright 2022 The Oak Authors.

#include "oak/config.h"

#include <assert.h>
#include <fstream>

#include "json/json.h"
#include "oak/common/format.h"
#include "oak/common/throw_delegate.h"
#include "oak/common/fs.h"

namespace oak {
// GetMasterProcessConfig(), GetWorkerProcessConfig()

#define OAK_ETC_DIR     "etc"
#define OAK_LOG_DIR     "log"
#define OAK_ADDONS_DIR  "addons"
#define OAK_GUARD_FILE  "rs.pid"
#define OAK_PROC_NAME   "Rs"
#define OAK_LOG_FILE    "rs.log"
#define OAK_CONF_FILE   "rs.json"
#define OAK_CRASH_FILE  "rs.crash.log"

namespace {
const ProcessConfig GetProcessConfigImpl() {
  ProcessConfig config;

  config.home       = GetRealPath("..");
  config.bin_dir    = GetCurrentDirectory();
  config.etc_dir    = config.home + "/" OAK_ETC_DIR;
  config.log_dir    = config.home + "/" OAK_LOG_DIR;
  config.addons_dir = config.home + "/" OAK_ADDONS_DIR;
  config.guard_file = "/tmp/oak/" OAK_GUARD_FILE;
  config.proc_name  = OAK_PROC_NAME;
  config.log_file   = config.log_dir + "/" OAK_LOG_FILE;
  config.conf_file  = config.etc_dir + "/" OAK_CONF_FILE;
  config.crash_file = config.log_dir + "/" OAK_CRASH_FILE;
  return config;
}

}  // anonymous namespace

const ProcessConfig& GetProcessConfig() {
  static const ProcessConfig config = GetProcessConfigImpl();
  return config;
}

namespace {

// ReadModuleConfig()

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

// WriteModuleConfig()

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

void ReadSourceConfig(SourceConfig* config, const Json2::Value& node) {
  if (node.isMember("comment"))
    config->comment = node["comment"].asString();

  if (node.isMember("num_threads"))
    config->num_threads = node["num_threads"].asUInt();

  if (node.isMember("modules"))
    ReadModuleConfig(&config->modules, node["modules"]);
}

void WriteSourceConfig(const SourceConfig& config, Json2::Value* node) {
  (*node)["comment"] = config.comment;
  (*node)["num_threads"] = config.num_threads;
  (*node)["modules"] = Json2::Value(Json2::objectValue);
  WriteModuleConfig(config.modules, &((*node)["modules"]));
}

void ReadParserConfig(ParserConfig* config, const Json2::Value& node) {
  if (node.isMember("comment"))
    config->comment = node["comment"].asString();

  if (node.isMember("num_threads"))
    config->num_threads = node["num_threads"].asUInt();

  if (node.isMember("modules"))
    ReadModuleConfig(&config->modules, node["modules"]);
}

void WriteParserConfig(const ParserConfig& config, Json2::Value* node) {
  (*node)["comment"] = config.comment;
  (*node)["num_threads"] = config.num_threads;
  (*node)["modules"] = Json2::Value(Json2::objectValue);
  WriteModuleConfig(config.modules, &((*node)["modules"]));
}

void ReadSinkConfig(SinkConfig* config, const Json2::Value& node) {
  if (node.isMember("comment"))
    config->comment = node["comment"].asString();

  if (node.isMember("num_threads"))
    config->num_threads = node["num_threads"].asUInt();

  if (node.isMember("modules"))
    ReadModuleConfig(&config->modules, node["modules"]);
}

void WriteSinkConfig(const SinkConfig& config, Json2::Value* node) {
  (*node)["comment"] = config.comment;
  (*node)["num_threads"] = config.num_threads;
  (*node)["modules"] = Json2::Value(Json2::objectValue);
  WriteModuleConfig(config.modules, &((*node)["modules"]));
}

}  // anonymous namespace

void ReadConfig(Config* config, const std::string& fname) {
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

  // Read source configuration
  if (node.isMember("source"))
    ReadSourceConfig(&config->source, node["source"]);

  // Read parser configuration
  if (node.isMember("parser"))
    ReadParserConfig(&config->parser, node["parser"]);

  // Read sink configuration
  if (node.isMember("sink"))
    ReadSinkConfig(&config->sink, node["sink"]);
}

void WriteConfig(const Config& config, const std::string& fname) {
  Json2::Value node(Json2::objectValue);

  node["comment"] = config.comment;
  node["log_method"] = config.log_method;

  node["modules"] = Json2::Value(Json2::objectValue);
  WriteModuleConfig(config.modules, &(node["modules"]));

  // Write source configuration
  node["source"] = Json2::Value(Json2::objectValue);
  WriteSourceConfig(config.source, &(node["source"]));

  // Write parser configuration
  node["parser"] = Json2::Value(Json2::objectValue);
  WriteParserConfig(config.parser, &(node["parser"]));

  // Write sink configuration
  node["sink"] = Json2::Value(Json2::objectValue);
  WriteSinkConfig(config.sink, &(node["sink"]));

  Json2::StyledWriter writer;
  std::string buffer = writer.write(node);
  File::MakeWritableFile(fname).Write(buffer);
}

}  // namespace oak
