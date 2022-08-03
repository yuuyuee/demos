// Copyright 2022 The Oak Authors.

#ifndef OAK_ADDONS_SOURCE_H_
#define OAK_ADDONS_SOURCE_H_

#include "oak/addons/parameter.h"

#ifdef __cplusplus
extern "C" {
#endif

struct source_context {
  char name[NAME_MAX];
  char work_directory[PATH_MAX];
  void* args;
};

struct source_ops {
  int (*open)(source_context*);
  ssize_t (*read)(source_context*, oak_buffer* buffer);
  void (*close)(source_context*);
};

#ifdef __cplusplus
}
#endif

#endif  /* OAK_ADDONS_SOURCE_H_ */
