/* Copyright 2022 The Oak Authors. */

#ifndef OAK_ADDONS_PUBLIC_BUFFER_H_
#define OAK_ADDONS_PUBLIC_BUFFER_H_

#include <stddef.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* oak_buffer_flags */

enum oak_buffer_flags {
  /* whether or not owned the underlying data */
  OAK_BUFFER_OWNED = 0x01
};

/* struct oak_buffer
 *
 * Used to create a buffer object to represent raw memory.*/

struct oak_buffer {
  void* ptr;
  size_t size;
  int flags;
  size_t capacity;
};

#define OAK_BUFFER_DFL_CAP 128

/* Copy c-style string @str with @size bytes to buffer @lhs. */
inline void oak_buffer_assign_str(struct oak_buffer* lhs,
                                  const void* str, size_t size) {
  assert((lhs->flags & OAK_BUFFER_OWNED) && "Require owned oak_buffer");
  if (lhs->capacity < size) {
    lhs->capacity = size;
    lhs->ptr = realloc(lhs->ptr, lhs->capacity + 1);
    if (!lhs->ptr)
      abort();
  }
  memcpy(lhs->ptr, str, size);
  lhs->size = size;
  ((unsigned char *) lhs->ptr)[lhs->size] = '\0';
}

#ifdef __cplusplus
}
#endif

#endif  /* OAK_ADDONS_PUBLIC_BUFFER_H_ */
