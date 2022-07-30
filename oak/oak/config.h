// Copyright 2022 The Oak Authors.

#ifndef OAK_CONFIG_H_
#define OAK_CONFIG_H_

#include <limits.h>
#include <sched.h>

namespace oak {

struct WorkConfig {
  int state;

  int socket_id;
  int core_id;
  cpu_set_t cpu_set;
};

#define OAK_MAX_WORKERS 128

struct Config {
  char config_path[PATH_MAX];
  WorkConfig work_config[OAK_MAX_WORKERS];
};

int LoadConfig(const char* fname);
}  // namespace oak

#endif  // OAK_CONFIG_H_
