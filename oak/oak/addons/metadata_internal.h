/* Copyright 2022 The Oak Authors. */

#ifndef OAK_ADDONS_METADATA_INTERNAL_H_
#define OAK_ADDONS_METADATA_INTERNAL_H_

#include "oak/addons/public/metadata.h"
#include "oak/addons/buffer_internal.h"
#include "oak/addons/dict_internal.h"

#ifdef __cplusplus
extern "C" {
#endif

inline void oak_meta_init(struct oak_metadata* meta) {
  oak_buffer_init(&meta->up_stream, OAK_BUFFER_DFL_CAP);
  oak_buffer_init(&meta->down_stream, OAK_BUFFER_DFL_CAP);
  oak_dict_init(&meta->common);
  oak_dict_init(&meta->fields);
  oak_dict_init(&meta->exts);
  meta->flags = 0;
}

inline void oak_meta_free(struct oak_metadata* meta) {
  oak_buffer_free(&meta->up_stream);
  oak_buffer_free(&meta->down_stream);
  oak_dict_free(&meta->common);
  oak_dict_free(&meta->fields);
  oak_dict_free(&meta->exts);
}

inline void oak_meta_clear(struct oak_metadata* meta) {
  oak_buffer_clear(&meta->up_stream);
  oak_buffer_clear(&meta->down_stream);
  oak_dict_clear(&meta->common);
  oak_dict_clear(&meta->fields);
  oak_dict_clear(&meta->exts);
  meta->flags = 0;
}

#ifdef __cplusplus
}
#endif

#endif  /* OAK_ADDONS_METADATA_INTERNAL_H_ */
