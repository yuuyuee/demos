// Copyright 2022 The Oak Authors.

#ifndef OAK_ADDONS_MODULE_H_
#define OAK_ADDONS_MODULE_H_

#include <string>

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
  ModuleBase();
  virtual ~ModuleBase();

  // Module unique ID
  virtual int id() const noexcept;

  // Module version
  virtual int version() const noexcept;

  // Module name also module entry
  virtual const std::string& name() const noexcept;

  // Module path
  virtual const std::string& path() const noexcept;

  // Module type
  virtual ModuleType type() const noexcept;

  // Module language type
  virtual LangType lang_type() const noexcept;
};

// Open C/C++ implemented module.
void* OpenCppModule(const std::string& entry, const std::string& path);

// Close C/C++ implemented module.
void CloseCppModule(void* handle);

// Open Python implemented module.
void* OpenPyModule(const std::string& entry, const std::string& path);

// Close Python implemented module.
void ClosePyModule(void* handle);

// Dump module.
void DumpModule(const ModuleBase& base);

}  // namespace oak
#endif  // OAK_ADDONS_MODULE_H_
