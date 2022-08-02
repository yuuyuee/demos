// Copyright 2022 The Oak Authors.

#ifndef OAK_ADDONS_SOURCE_H_
#define OAK_ADDONS_SOURCE_H_

#include "oak/addons/common.h"

#ifdef __cplusplus
extern "C" {
#endif

struct source_ops {
  int (*open)(context*);
  ssize_t (*read)(context*, void* buffer, size_t buffer_size);
  void (*close)(context*);
};

#ifdef __cplusplus
}
#endif

#endif  /* OAK_ADDONS_SOURCE_H_ */
