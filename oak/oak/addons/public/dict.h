/* Copyright 2022 The Oak Authors. */

#ifndef OAK_ADDONS_PUBLIC_DICT_H_
#define OAK_ADDONS_PUBLIC_DICT_H_

#include <sys/types.h>
#include <stdlib.h>
#include <assert.h>
#include <stddef.h>

#include "oak/addons/public/compiler.h"
#include "oak/addons/public/platform.h"
#include "oak/addons/public/buffer.h"

#ifdef __cplusplus
extern "C" {
#endif

/* struct oak_dict
 *
 * Used to storing key:value pairs. */

#define OAK_DICT_DFL_CAP 128

struct oak_dict_entry {
  struct oak_buffer key;
  struct oak_buffer value;
};

inline void oak_dict_entry_init(struct oak_dict_entry* entry) {
  oak_buffer_init(&entry->key, OAK_BUFFER_DFL_CAP);
  oak_buffer_init(&entry->value, OAK_BUFFER_DFL_CAP);
}

inline void oak_dict_entry_free(struct oak_dict_entry* entry) {
  oak_buffer_free(&entry->key);
  oak_buffer_free(&entry->value);
}

struct oak_dict {
  struct oak_dict_entry* elems[OAK_DICT_DFL_CAP];
  size_t size;
};

inline void oak_dict_init(struct dict* dict) {

}

inline void oak_dict_free(struct dict* dict) {

}



#ifdef __cplusplus
}
#endif

#endif  /* OAK_ADDONS_PUBLIC_DICT_H_ */