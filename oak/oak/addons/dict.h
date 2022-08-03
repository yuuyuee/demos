// Copyright 2022 The Oak Authors.

#ifndef OAK_ADDONS_DICT_H_
#define OAK_ADDONS_DICT_H_

#include <sys/types.h>
#include <stdlib.h>
#include <assert.h>
#include <stddef.h>

#include "oak/addons/internal/compiler.h"
#include "oak/addons/internal/platform.h"
#include "oak/addons/buffer.h"

#ifdef __cplusplus
extern "C" {
#endif

/* struct oak_dict
 *
 * Used to storing key:value pairs. */

#define OAK_DICT_CAPABILITY 512

struct oak_dict_entry {
  struct oak_buffer key;
  struct oak_buffer value;
};

inline void oak_dict_entry_init(struct oak_dict_entry* entry) {
  oak_buffer_init(&entry->key);
  oak_buffer_init(&entry->value);
}

inline void oak_dict_entry_free(struct oak_dict_entry* entry) {
  oak_buffer_free(&entry->key);
  oak_buffer_free(&entry->value);
}

struct oak_dict {
  struct oak_dict_entry* elems[OAK_DICT_CAPABILITY];
  size_t size;
  size_t capability;
};



#ifdef __cplusplus
}
#endif

#endif  /* OAK_ADDONS_DICT_H_ */
