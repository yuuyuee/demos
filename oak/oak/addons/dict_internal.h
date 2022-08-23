/* Copyright 2022 The Oak Authors. */

#ifndef OAK_ADDONS_DICT_INTERNAL_H_
#define OAK_ADDONS_DICT_INTERNAL_H_

#include <assert.h>

#include "oak/addons/public/dict.h"
#include "oak/addons/buffer_internal.h"

#ifdef __cplusplus
extern "C" {
#endif

inline void oak_dict_entry_init(struct oak_dict_entry* entry) {
  oak_buffer_init(&entry->key, OAK_BUFFER_DFL_CAP);
  oak_buffer_init(&entry->value, OAK_BUFFER_DFL_CAP);
}

inline void oak_dict_entry_init2(struct oak_dict_entry* entry) {
  oak_buffer_init2(&entry->key, NULL, 0);
  oak_buffer_init2(&entry->value, NULL, 0);
}

inline void oak_dict_entry_free(struct oak_dict_entry* entry) {
  oak_buffer_free(&entry->key);
  oak_buffer_free(&entry->value);
}

inline void oak_dict_init(struct oak_dict* dict) {
  for (size_t i = 0; i < OAK_DICT_DFL_CAP; ++i)
    oak_dict_entry_init(&(dict->elems[i]));
  dict->size = 0;
}

inline void oak_dict_init2(struct oak_dict* dict) {
  for (size_t i = 0; i < OAK_DICT_DFL_CAP; ++i)
    oak_dict_entry_init2(&(dict->elems[i]));
  dict->size = 0;
}

inline void oak_dict_ref(struct oak_dict* dict,
                         const char* key, size_t key_len,
                         const char* value, size_t value_len) {
  assert((dict->elems[dict->size].key.flags & OAK_BUFFER_OWNED) == 0);
  assert((dict->elems[dict->size].value.flags & OAK_BUFFER_OWNED) == 0);
  oak_buffer_ref(&(dict->elems[dict->size].key), key, key_len);
  oak_buffer_ref(&(dict->elems[dict->size].value), value, value_len);
  ++dict->size;
}

inline void oak_dict_clear(struct oak_dict* dict) {
  dict->size = 0;
}

inline void oak_dict_free(struct oak_dict* dict) {
  for (size_t i = 0; i < OAK_DICT_DFL_CAP; ++i)
    oak_dict_entry_free(&(dict->elems[i]));
  dict->size = 0;
}

#ifdef __cplusplus
}
#endif

#endif  /* OAK_ADDONS_DICT_INTERNAL_H_ */
