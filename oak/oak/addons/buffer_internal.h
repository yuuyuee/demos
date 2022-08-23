/* Copyright 2022 The Oak Authors. */

#ifndef OAK_ADDONS_BUFFER_INTERNAL_H_
#define OAK_ADDONS_BUFFER_INTERNAL_H_

#include "oak/addons/public/buffer.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Initialize the object.
 * Note: which @buffer object must be an uninitialized object. */
void oak_buffer_init(struct oak_buffer* buffer, size_t capacity);

/* Initialize the object with @len bytes string @str, but does not
 * owned the underlying data.
 * Note: which @buffer object must be an uninitialized object. */
void oak_buffer_init2(struct oak_buffer* buffer,
                      const void* str, size_t len);

/* Destruct object. */
void oak_buffer_free(struct oak_buffer* buffer);

/* Clear object. */
inline void oak_buffer_clear(struct oak_buffer* buffer) {
  buffer->size = 0;
}

/* Checks whether or not the buffer is empty. */
inline int oak_buffer_empty(const struct oak_buffer* buffer) {
  return buffer->size == 0;
}

/* Reference size bytes string named str, does not owned the */
/* underlying data. */
inline void oak_buffer_ref(struct oak_buffer* buffer,
                           const void* str, size_t size) {
  assert((buffer->flags & ~OAK_BUFFER_OWNED) == 0 &&
          "Dont require owned oak_buffer");
  buffer->ptr = (void*) str;
  buffer->size = size;
}

/* Move buffer ownership of @rhs to @lhs. */
void oak_buffer_move(struct oak_buffer* lhs, struct oak_buffer* rhs);

/* Copy buffer @rhs to @lhs. */
void oak_buffer_assign(struct oak_buffer* lhs, const struct oak_buffer* rhs);

/* Append buffer @rhs to @rhs. */
void oak_buffer_append(struct oak_buffer* lhs, const struct oak_buffer* rhs);

/* Append c-style string @str with @size bytes to buffer @lhs. */
void oak_buffer_append_str(struct oak_buffer* lhs,
                           const void* str, size_t size);

/* Compare equal between @lhs and @rhs. */
int oak_buffer_compare(const oak_buffer* lhs, const oak_buffer* rhs);

/* Compare equal between @lhs and string @str with @size bytes. */
int oak_buffer_compare_str(const oak_buffer* lhs,
                           const void* str, size_t size);

#ifdef __cplusplus
}
#endif

#endif  /* OAK_ADDONS_BUFFER_INTERNAL_H_ */
