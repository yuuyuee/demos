/* Copyright 2022 The Oak Authors. */

#ifndef OAK_ADDONS_PUBLIC_METADATA_H_
#define OAK_ADDONS_PUBLIC_METADATA_H_

#include "oak/addons/public/buffer.h"
#include "oak/addons/public/dict.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OAK_ID_MAX 10

struct oak_metadata {
  int ids[OAK_ID_MAX];
  int num_ids;
  struct oak_buffer up_stream;
  struct oak_buffer down_stream;
  struct oak_dict common;
  struct oak_dict fields;
  struct oak_dict exts;
  int flags;
  int padding_;
};

#ifdef __cplusplus
}
#endif

#endif  /* OAK_ADDONS_PUBLIC_METADATA_H_ */
