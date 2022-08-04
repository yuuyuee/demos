/* Copyright 2022 The Oak Authors. */

#ifndef OAK_ADDONS_PUBLIC_PARSE_H_
#define OAK_ADDONS_PUBLIC_PARSE_H_

#include "oak/addons/public/compiler.h"
#include "oak/addons/public/platform.h"
#include "oak/addons/public/buffer.h"

#ifdef __cplusplus
extern "C" {
#endif

struct oak_parse_context {
  char name[NAME_MAX];
  char work_directory[PATH_MAX];
  void* args;
};

typedef void (*oak_dict_set)(struct oak_buffer_ref* key,
                             struct oak_buffer_ref* value);

struct oak_parse_ops {
  int (*open)(struct oak_parse_context* ctx);
  void (*parse)(struct oak_parse_context* ctx,
                const struct oak_buffer_ref* up_stream,
                const struct oak_buffer_ref* down_stream,
                oak_dict_set set);
  void (*close)(struct parse_context* ctx);
  unsigned int (version)();
};

#ifdef __cplusplus
}
#endif

#endif  /* OAK_ADDONS_PUBLIC_PARSE_H_ */
