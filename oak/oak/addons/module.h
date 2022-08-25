// Copyright 2022 The Oak Authors.

#ifndef OAK_ADDONS_MODULE_H_
#define OAK_ADDONS_MODULE_H_

#include <string>
#include <unordered_map>

namespace oak {

// Module type
enum ModuleType {
  MODULE_TYPE_UNKNOWN = 0,
  MODULE_TYPE_SOURCE = 0x01,
  MODULE_TYPE_PARSER = 0x02,
  MODULE_TYPE_SINK = 0x03,
  MODULE_TYPE_EVENT = 0x04
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
  uint64_t id() const noexcept {
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
  uint64_t id_;
  int version_;
  std::string name_;
  std::string path_;
  ModuleType type_;
  LangType lang_type_;
};

}  // namespace oak
#endif  // OAK_ADDONS_MODULE_H_
