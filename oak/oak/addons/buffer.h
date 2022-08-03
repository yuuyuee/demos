// Copyright 2022 The Oak Authors.

#ifndef OAK_ADDONS_BUFFER_H_
#define OAK_ADDONS_BUFFER_H_

#include <sys/types.h>
#include <stdlib.h>
#include <assert.h>
#include <stddef.h>
#include <string.h>

#include "oak/addons/internal/compiler.h"
#include "oak/addons/internal/platform.h"

#ifdef __cplusplus
extern "C" {
#endif

/* struct oak_buffer
 *
 * Used to create a buffer object to represent raw memory.*/

struct oak_buffer {
  void* ptr;
  size_t size;
  size_t capability;
};

/* Initialize the object.
 * Note: which @buffer object must be an uninitialized object. */
inline void oak_buffer_init(struct oak_buffer* buffer, size_t capability) {
  buffer->size = 0;
  buffer->capability = capability;
  buffer->ptr = NULL;
  if (buffer->capability > 0) {
    buffer->ptr = malloc(buffer->capability);
    if (!buffer->ptr)
      abort();
  }
}

/* Destruct object. */
inline void oak_buffer_free(struct oak_buffer* buffer) {
  free(buffer->ptr);
  buffer->ptr = NULL;
  buffer->size = 0;
  buffer->capability = 0;
}

/* Clear object. */
inline void oak_buffer_clear(struct oak_buffer* buffer) {
  buffer->size = 0;
}

/* Test whether it is empty. */
inline int oak_buffer_empty(const struct oak_buffer* buffer) {
  return buffer->size == 0;
}

/* Move buffer ownership of @rhs to @lhs. */
inline void oak_buffer_move(struct oak_buffer* lhs,
                            struct oak_buffer* rhs) {
  oak_buffer_free(lhs);
  *lhs = *rhs;
  oak_buffer_init(rhs, 0);
}

/* Copy buffer @rhs to @lhs. */
inline void oak_buffer_assign(struct oak_buffer* lhs,
                              const struct oak_buffer* rhs) {
  if (lhs->capability < rhs->size) {
    lhs->capability = rhs->size;
    lhs->ptr = realloc(lhs->ptr, lhs->capability);
    if (!lhs->ptr)
      abort();
  }
  memcpy(lhs->ptr, rhs->ptr, rhs->size);
  lhs->size = rhs->size;
}

/* Copy c-style string @str with @size bytes to buffer @lhs. */
inline void oak_buffer_assign_str(struct oak_buffer* lhs,
                                  const void* str,
                                  size_t size) {
  if (lhs->capability < size) {
    lhs->capability = size;
    lhs->ptr = realloc(lhs->ptr, lhs->capability);
    if (!lhs->ptr)
      abort();
  }
  memcpy(lhs->ptr, str, size);
  lhs->size = size;
}

/* Append buffer @rhs to @rhs. */
inline void oak_buffer_append(struct oak_buffer* lhs,
                              const struct oak_buffer* rhs) {
  if (lhs->capability < lhs->size + rhs->size) {
    lhs->capability = lhs->size + rhs->size;
    lhs->capability = lhs->capability + lhs->capability / 2;
    lhs->ptr = realloc(lhs->ptr, lhs->capability);
    if (lhs->ptr == NULL)
      abort();
  }
  memcpy(((unsigned char*) lhs->ptr) + lhs->size, rhs->ptr, rhs->size);
  lhs->size += rhs->size;
}

/* Append c-style string @str with @size bytes to buffer @lhs. */
inline void oak_buffer_append_str(struct oak_buffer* lhs,
                                  const void* str,
                                  size_t size) {
  if (lhs->capability < lhs->size + size) {
    lhs->capability = lhs->size + size;
    lhs->capability = lhs->capability + lhs->capability / 2;
    lhs->ptr = realloc(lhs->ptr, lhs->capability);
    if (lhs->ptr == NULL)
      abort();
  }
  memcpy(((unsigned char*) lhs->ptr) + lhs->size, str, size);
  lhs->size += size;
}

/* Compare equal between @lhs and @rhs. */
inline int oak_buffer_compare(const oak_buffer* lhs,
                              const oak_buffer* rhs) {
  return (lhs->size == rhs->size &&
          memcmp(lhs->ptr, rhs->ptr, lhs->size) == 0)
            ? 0 : 1;
}

/* struct oak_piece, oak_const_piece
 *
 * like as oak_buffer but it does not own the underlying data.
 * and it does not modify the underlying data. */

struct oak_piece {
  const void* ptr;
  size_t size;
};

#define OAK_MEMBER_SIZE(type, field) (sizeof(((type*) 0)->field))

/* Create an referrence from the oak_buffer. */
inline struct oak_piece oak_as_piece(const struct oak_buffer* buffer) {
  assert(offsetof(struct oak_buffer, ptr) ==
         offsetof(struct oak_piece, ptr) &&
         OAK_MEMBER_SIZE(struct oak_buffer, ptr) ==
         OAK_MEMBER_SIZE(struct oak_piece, ptr));
  assert(offsetof(struct oak_buffer, size) ==
         offsetof(struct oak_piece, size) &&
         OAK_MEMBER_SIZE(struct oak_buffer, size) ==
         OAK_MEMBER_SIZE(struct oak_piece, size));

  return *(const struct oak_piece*) buffer;
}

/* Create an referrence from the c-style string. */
inline struct oak_piece
oak_as_piece_str(const char* str, size_t size) {
  const struct oak_piece piece = {
    .ptr = str,
    .size = size
  };
  return piece;
}

/* Compare equal between @lhs and @rhs. */
inline int oak_piece_compare(const oak_piece lhs,
                             const oak_piece rhs) {
  return (lhs.size == rhs.size &&
          memcmp(lhs.ptr, rhs.ptr, lhs.size) == 0)
            ? 0 : 1;
}

/* Test whether it is empty. */
inline int oak_piece_empty(const struct oak_piece piece) {
  return piece.size == 0;
}


#ifdef __cplusplus
}
#endif

#endif  /* OAK_ADDONS_BUFFER_H_ */
