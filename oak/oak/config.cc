// Copyright 2022 The Oak Authors.

#include "oak/config.h"
#include "json/json.h"
namespace oak {

Config::Config(const char* fname)
    : multiple_process(false) {
  auto feature = Json2::Feature::strictMode();
  Json2::Reader reader(feature);
  Json2::Value root;
  Json2::IStream in(fname);

  if (!reader.read(in, root, false)) {
    OAK_FATAL("Read %s failed: %s\n",
      fname, reader.getFormattedErrorMessages().c_str());
  }

  multiple_process = root["multiple_process"].asBool();
}

}  // namespace oak
