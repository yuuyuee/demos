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
#define OAK_MAX_PRONAME 128

struct Config {
  bool multiple_process;

  explicit Config(const char* fname);
};

}  // namespace oak

#endif  // OAK_CONFIG_H_
