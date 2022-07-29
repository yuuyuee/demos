// Copyright 2022 The Oak Authors.

#ifndef OAK_CONFIG_CONFIG_H_
#define OAK_CONFIG_CONFIG_H_

namespace oak {

struct WorkConfig {

  int state
};

#define OAK_MAX_WORKERS 128

struct Config {

  WorkConfig work_config[OAK_MAX_WORKERS];
};

int LoadConfig(const char* fname);
}  // namespace oak

#endif  // OAK_CONFIG_CONFIG_H_
