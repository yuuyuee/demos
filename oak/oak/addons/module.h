// Copyright 2022 The Oak Authors.

#ifndef OAK_ADDONS_MODULE_H_
#define OAK_ADDONS_MODULE_H_

#include <string>
#include <unordered_map>

#include "oak/addons/public/event.h"
#include "oak/addons/public/source.h"
#include "oak/addons/public/parser.h"
#include "oak/addons/public/sink.h"

namespace oak {

// Module type
enum ModuleType {
  MODULE_TYPE_UNKNOWN = 0,
  MODULE_TYPE_SOURCE = OAK_MODULE_SOURCE,
  MODULE_TYPE_PARSER = OAK_MODULE_PARSER,
  MODULE_TYPE_SINK = OAK_MODULE_SINK,
  MODULE_TYPE_EVENT = OAK_MODULE_EVENT
};

constexpr const char* ModuleTypeName(ModuleType type) {
  return type == MODULE_TYPE_SOURCE ? "Source"
      : type == MODULE_TYPE_PARSER ? "Parser"
      : type == MODULE_TYPE_SINK ? "Sink"
      : type == MODULE_TYPE_EVENT ? "Event"
      : "Unknown";
}

// Module language type
enum LangType {
  LANG_TYPE_UNKNOWN = 0,
  LANG_TYPE_C_CPP = 1,
  LANG_TYPE_PYTHON = 2,
};

constexpr const char* LangTypeName(LangType type) {
  return type == LANG_TYPE_C_CPP ? "C/C++"
      : type == LANG_TYPE_PYTHON ? "Python"
      : "Unknown";
}

// ModuleBase
class ModuleBase {
 public:
  virtual ~ModuleBase();

  // Dump the module information.
  void Dump() const noexcept;

  // Module unique ID
  int id() const noexcept {
    return id_;
  }

  // Module version
  int version() const noexcept {
    return version_;
  }

  // Module name also module entry
  const std::string& name() const& noexcept {
    return name_;
  }

  // Module path
  const std::string& path() const& noexcept {
    return path_;
  }

  // Module type
  ModuleType type() const noexcept {
    return type_;
  }

  // Module language type
  LangType lang_type() const noexcept {
    return lang_type_;
  }

 protected:
  ModuleBase() noexcept;
  ModuleBase(int id, const std::string& name, const std::string& path) noexcept;

 protected:
  int id_;
  int version_;
  std::string name_;
  std::string path_;
  ModuleType type_;
  LangType lang_type_;
};

using Dict = std::unordered_map<std::string, std::string>;

struct ModuleArguments {
  int id;
  std::string module_name;
  std::string module_path;
  const Dict config;
};

}  // namespace oak
#endif  // OAK_ADDONS_MODULE_H_
