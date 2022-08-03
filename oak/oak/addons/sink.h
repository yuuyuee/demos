// Copyright 2022 The Oak Authors.

#ifndef OAK_ADDONS_SINK_H_
#define OAK_ADDONS_SINK_H_

#include "oak/addons/parameter.h"

#ifdef __cplusplus
extern "C" {
#endif

struct oak_sink_context {
  char name[NAME_MAX];
  char work_directory[PATH_MAX];
  void* args;
};

struct oak_sink_ops {
  int (*open)(struct oak_sink_context*      /* ctx */);
  ssize_t (*write)(struct oak_sink_context* /* ctx */,
                   void* buffer,
                   size_t buffer_size);
  void (*close)(struct oak_sink_context*);
};

#ifdef __cplusplus
}
#endif

#endif  /* OAK_ADDONS_SINK_H_ */
