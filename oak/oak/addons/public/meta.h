/* Copyright 2022 The Oak Authors. */

#ifndef OAK_ADDONS_PUBLIC_META_H_
#define OAK_ADDONS_PUBLIC_META_H_

#include "oak/addons/public/compiler.h"
#include "oak/addons/public/platform.h"
#include "oak/addons/public/buffer.h"
#include "oak/addons/public/dict.h"

#ifdef __cplusplus
extern "C" {
#endif

struct oak_meta_data {
  struct oak_buffer up_stream;
  struct oak_buffer down_stream;
  struct oak_dict universal;
  struct oak_dict fields;
  struct oak_dict exts;
  bool flag;
};

#ifdef __cplusplus
}
#endif

#endif  /* OAK_ADDONS_PUBLIC_META_H_ */
