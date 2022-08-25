// Copyright 2022 The Oak Authors.

#include "oak/addons/module.h"
#include "oak/logging/logging.h"

using std::string;

namespace oak {

ModuleBase::ModuleBase(int id, const string& name, const string& path) noexcept
    : id_(id), version_(0), name_(name), path_(path),
      type_(MODULE_TYPE_UNKNOWN),
      lang_type_(LANG_TYPE_UNKNOWN) {}

ModuleBase::ModuleBase() noexcept: ModuleBase(0, "", "") {}

ModuleBase::~ModuleBase() {}

void ModuleBase::Dump() const noexcept {
  OAK_INFO(
      "Module ID: %ld\n"
      "Module Version: %d\n"
      "Module Entry: %s\n"
      "Module Path: %s\n"
      "Module Type: %s\n"
      "Module Language Type: %s\n"
      "Module loaded successfully.\n",
      id(),
      version(),
      name().c_str(),
      path().empty() ? "builtin" : path().c_str(),
      ModuleTypeName(type()),
      LangTypeName(lang_type()));
}

}  // namespace oak
