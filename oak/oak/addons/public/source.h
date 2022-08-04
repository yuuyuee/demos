/* Copyright 2022 The Oak Authors. */

#ifndef OAK_ADDONS_PUBLIC_SOURCE_H_
#define OAK_ADDONS_PUBLIC_SOURCE_H_

#include "oak/addons/public/compiler.h"
#include "oak/addons/public/platform.h"
#include "oak/addons/public/buffer.h"

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
  unsigned int (version)();
};

#ifdef __cplusplus
}
#endif

#endif  /* OAK_ADDONS_PUBLIC_SOURCE_H_ */
