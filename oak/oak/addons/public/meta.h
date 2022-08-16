/* Copyright 2022 The Oak Authors. */

#ifndef OAK_ADDONS_PUBLIC_META_H_
#define OAK_ADDONS_PUBLIC_META_H_

#include "oak/addons/public/buffer.h"
#include "oak/addons/public/dict.h"

#ifdef __cplusplus
extern "C" {
#endif

struct oak_metadata {
  struct oak_buffer up_stream;
  struct oak_buffer down_stream;

  /* Below the internal fields */
  struct oak_dict universal;
  struct oak_dict fields;
  struct oak_dict exts;
  int flags;
  int padding_;
};

inline void oak_meta_init(struct oak_metadata* meta) {
  oak_buffer_init(&meta->up_stream, OAK_BUFFER_DFL_CAP);
  oak_buffer_init(&meta->down_stream, OAK_BUFFER_DFL_CAP);
  oak_dict_init(&meta->universal);
  oak_dict_init(&meta->fields);
  oak_dict_init(&meta->exts);
  meta->flags = 0;
}

inline void oak_meta_free(struct oak_metadata* meta) {
  oak_buffer_free(&meta->up_stream);
  oak_buffer_free(&meta->down_stream);
  oak_dict_free(&meta->universal);
  oak_dict_free(&meta->fields);
  oak_dict_free(&meta->exts);
}

inline void oak_meta_clear(struct oak_metadata* meta) {
  oak_buffer_clear(&meta->up_stream);
  oak_buffer_clear(&meta->down_stream);
  oak_dict_clear(&meta->universal);
  oak_dict_clear(&meta->fields);
  oak_dict_clear(&meta->exts);
  meta->flags = 0;
}

#ifdef __cplusplus
}
#endif

#endif  /* OAK_ADDONS_PUBLIC_META_H_ */
