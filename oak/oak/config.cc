// Copyright 2022 The Oak Authors.

#include "oak/config.h"
#include <fstream>
#include "json/json.h"
#include "oak/logging/logging.h"
namespace oak {

Config::Config(const char* fname)
    : multiple_process(false) {
  std::ifstream in(fname);
  if (!in) return;

  auto features = Json2::Features::strictMode();
  Json2::Reader reader(features);
  Json2::Value root;

  if (!reader.parse(in, root, false)) {
    OAK_PANIC("Read %s failed: %s\n",
      fname, reader.getFormattedErrorMessages().c_str());
  }

  if (root.isMember("multiple_process")) {
    multiple_process = root["multiple_process"].asBool();
  }
}

}  // namespace oak
