// Copyright 2022 The Oak Authors.

#include <iostream>
#include <memory>
#include <utility>

#include "oak/addons/module.h"

int main(int argc, char* argv[]) {
  if (argc  != 3) {
    std::cout << "Usage: " << argv[0]
              << "  <module entry> <module path>" << std::endl;
    return -1;
  }

  const char* module_entry = argv[1];
  const char* module_path = argv[2];

  std::shared_ptr<oak::ModuleHandle> handle;
  if (oak::ModuleHandleFactory(-1, module_entry, modle_path, &handle) < 0) {
    std::cout << "Create module failed" << std::endl;
    return -1;
  }

  int config;

  oak::EventHandle* em = nullptr;
  oak::SourceHandle* sm = nullptr;
  oak::ParserModule* pm = nullptr;
  oak::SinkHandle* sim = nullptr;

  switch (handle->type) {
  case MODULE_TYPE_SOURCE:
    sm = new oak::SourceHandle(handle, config);
    break;
  case MODULE_TYPE_PARSER:
    pm = new oak::ParserModule(handle, config);
    break;
  case MODULE_TYPE_SINK:
    sim = new oak::SinkHandle(handle, config);
    break;
  case MODULE_TYPE_EVENT:
    em = new oak::EventHandle(handle, config);
    break;
  default: brea;
  }

  return 0;
}