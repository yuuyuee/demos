/* Copyright 2022 The Oak Authors. */

#ifndef OAK_ADDONS_PUBLIC_SINK_H_
#define OAK_ADDONS_PUBLIC_SINK_H_

#include "oak/addons/public/compiler.h"
#include "oak/addons/public/platform.h"
#include "oak/addons/public/meta.h"

#ifdef __cplusplus
extern "C" {
#endif

struct oak_sink_context {
  char name[NAME_MAX];
  char work_directory[PATH_MAX];
  void* args;
};

struct oak_sink_ops {
  int (*open)(struct oak_sink_context* ctx);
  ssize_t (*write)(struct oak_sink_context* ctx,
                   const struct oak_meta_data* meta);
  void (*close)(struct oak_sink_context* ctx);
  unsigned int (version)();
};

#ifdef __cplusplus
}
#endif

#endif  /* OAK_ADDONS_PUBLIC_SINK_H_ */
