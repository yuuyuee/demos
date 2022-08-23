/* Copyright 2022 The Oak Authors. */

#ifndef OAK_ADDONS_PUBLIC_DICT_H_
#define OAK_ADDONS_PUBLIC_DICT_H_

#include <stddef.h>
#include "oak/addons/public/buffer.h"

#ifdef __cplusplus
extern "C" {
#endif

/* struct oak_dict
 *
 * Used to storing key:value pairs. */

#define OAK_DICT_DFL_CAP 64

struct oak_dict_entry {
  struct oak_buffer key;
  struct oak_buffer value;
};

struct oak_dict {
  struct oak_dict_entry elems[OAK_DICT_DFL_CAP];
  size_t size;
};

inline void oak_dict_add(struct oak_dict* dict,
                         const char* key, size_t key_len,
                         const char* value, size_t value_len) {
  oak_buffer_assign_str(&(dict->elems[dict->size].key), key, key_len);
  oak_buffer_assign_str(&(dict->elems[dict->size].value), value, value_len);
  ++dict->size;
}

#ifdef __cplusplus
}
#endif

#endif  /* OAK_ADDONS_PUBLIC_DICT_H_ */
