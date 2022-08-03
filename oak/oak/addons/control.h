// Copyright 2022 The Oak Authors.

#ifndef OAK_ADDONS_CONTROL_H_
#define OAK_ADDONS_CONTROL_H_

#include "oak/addons/parameter.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef OAK_NAME_MAX
# define OAK_NAME_MAX 255
#endif

#ifndef OAK_PATH_MAX
# define OAK_PATH_MAX 4096
#endif

struct oak_control_context {
  char name[OAK_NAME_MAX];
  char work_directory[OAK_PATH_MAX];
  void* args;
};

struct oak_control_ops {
  int (*open)(struct oak_control_context*);
  int (*control)(struct oak_control_context*, const struct stream*);
  void (*close)(struct oak_control_context*);
};

#ifdef __cplusplus
}
#endif

#endif  /* OAK_ADDONS_CONTROL_H_ */
