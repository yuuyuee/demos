/* Copyright 2022 The Oak Authors. */

#ifndef OAK_ADDONS_METADATA_INTERNAL_H_
#define OAK_ADDONS_METADATA_INTERNAL_H_

#include "oak/addons/public/metadata.h"
#include "oak/addons/buffer_internal.h"
#include "oak/addons/dict_internal.h"

#ifdef __cplusplus
extern "C" {
#endif

inline void oak_metadata_init(struct oak_metadata* meta) {
  meta->num_tag = 0;
  for (int i = 0; i < OAK_META_TAG_MAX; ++i)
    oak_dict_init(&(meta->tag[i].fields));
  oak_buffer_init(&meta->up_stream, OAK_BUFFER_DFL_CAP);
  oak_buffer_init(&meta->down_stream, OAK_BUFFER_DFL_CAP);
  oak_buffer_init(&meta->attachment, OAK_BUFFER_DFL_CAP);
  oak_dict_init(&meta->common);
  oak_dict_init(&meta->fields);
  oak_dict_init(&meta->extens);
}

inline void oak_metadata_free(struct oak_metadata* meta) {
  for (int i = 0; i < OAK_META_TAG_MAX; ++i)
    oak_dict_free(&(meta->tag[i].fields));
  oak_buffer_free(&meta->up_stream);
  oak_buffer_free(&meta->down_stream);
  oak_buffer_free(&meta->attachment);
  oak_dict_free(&meta->common);
  oak_dict_free(&meta->fields);
  oak_dict_free(&meta->extens);
}

inline void oak_metadata_clear(struct oak_metadata* meta) {
  meta->num_tag = 0;
  for (int i = 0; i < OAK_META_TAG_MAX; ++i)
    oak_dict_clear(&(meta->tag[i].fields));
  oak_buffer_clear(&meta->up_stream);
  oak_buffer_clear(&meta->down_stream);
  oak_buffer_clear(&meta->attachment);
  oak_dict_clear(&meta->common);
  oak_dict_clear(&meta->fields);
  oak_dict_clear(&meta->extens);
}

#ifdef __cplusplus
}
#endif

#endif  /* OAK_ADDONS_METADATA_INTERNAL_H_ */
