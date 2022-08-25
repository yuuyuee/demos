/* Copyright 2022 The Oak Authors. */

#ifndef OAK_ADDONS_PUBLIC_METADATA_H_
#define OAK_ADDONS_PUBLIC_METADATA_H_

#include "oak/addons/public/buffer.h"
#include "oak/addons/public/dict.h"

#ifdef __cplusplus
extern "C" {
#endif

/* task type */
#define OAK_TASK_TYPE_PARSE   1
#define OAK_TASK_TYPE_CTRL    2

struct oak_metadata {
  uint64_t parser_id;
  uint64_t task_id;
  int task_type;
  struct oak_buffer up_stream;
  struct oak_buffer down_stream;
  struct oak_buffer attachment;
  struct oak_dict common;
  struct oak_dict fields;
  struct oak_dict communication;
  struct oak_dict extens_fields;
};

#ifdef __cplusplus
}
#endif

#endif  /* OAK_ADDONS_PUBLIC_METADATA_H_ */
