// Copyright 2022 The Oak Authors.

#ifndef OAK_ADDONS_PARSE_H_
#define OAK_ADDONS_PARSE_H_

#include "oak/addons/parameter.h"
#include "oak/addons/buffer.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef NAME_MAX
# define NAME_MAX 255
#endif

#ifndef PATH_MAX
# define PATH_MAX 4096
#endif

struct oak_parse_context {
  char name[NAME_MAX];
  char work_directory[PATH_MAX];
  void* args;
};

typedef void (*oak_dict_set)(struct oak_buffer* key,
                             struct oak_buffer* value);

struct oak_parse_ops {
  int (*open)(struct oak_parse_context* ctx);
  void (*parse)(struct oak_parse_context* ctx,
                const struct oak_buffer_ref* up_stream,
                const struct oak_buffer_ref* down_stream,
                oak_dict_set set);
  void (*close)(struct parse_context* ctx);
};

#ifdef __cplusplus
}
#endif

#endif  /* OAK_ADDONS_PARSE_H_ */
