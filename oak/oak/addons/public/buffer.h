/* Copyright 2022 The Oak Authors. */

#ifndef OAK_ADDONS_PUBLIC_BUFFER_H_
#define OAK_ADDONS_PUBLIC_BUFFER_H_

#include <sys/types.h>
#include <stdlib.h>
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include <limits.h>

#ifdef __cplusplus
extern "C" {
#endif

/* struct oak_buffer
 *
 * Used to create a buffer object to represent raw memory.*/

struct oak_buffer {
  void* ptr;
  size_t size;
  void* padding_;
  size_t capacity;
};

/* Initialize the object.
 * Note: which @buffer object must be an uninitialized object. */
inline void oak_buffer_init(struct oak_buffer* buffer, size_t capacity) {
  buffer->size = 0;
  buffer->padding_ = (void*) 0x45444F4344414544UL;   // NOLINT
  buffer->capacity = capacity;
  buffer->ptr = NULL;
  if (buffer->capacity > 0) {
    buffer->ptr = malloc(buffer->capacity);
    if (!buffer->ptr)
      abort();
  }
}

/* Destruct object. */
inline void oak_buffer_free(struct oak_buffer* buffer) {
  free(buffer->ptr);
  buffer->ptr = NULL;
  buffer->size = 0;
  buffer->capacity = 0;
}

/* Clear object. */
inline void oak_buffer_clear(struct oak_buffer* buffer) {
  buffer->size = 0;
}

/* Checks whether or not the buffer is empty. */
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
  if (lhs->capacity < rhs->size) {
    lhs->capacity = rhs->size;
    lhs->ptr = realloc(lhs->ptr, lhs->capacity);
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
  if (lhs->capacity < size) {
    lhs->capacity = size;
    lhs->ptr = realloc(lhs->ptr, lhs->capacity);
    if (!lhs->ptr)
      abort();
  }
  memcpy(lhs->ptr, str, size);
  lhs->size = size;
}

/* Append buffer @rhs to @rhs. */
inline void oak_buffer_append(struct oak_buffer* lhs,
                              const struct oak_buffer* rhs) {
  if (lhs->capacity < lhs->size + rhs->size) {
    lhs->capacity = lhs->size + rhs->size;
    lhs->capacity = lhs->capacity + lhs->capacity / 2;
    lhs->ptr = realloc(lhs->ptr, lhs->capacity);
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
  if (lhs->capacity < lhs->size + size) {
    lhs->capacity = lhs->size + size;
    lhs->capacity = lhs->capacity + lhs->capacity / 2;
    lhs->ptr = realloc(lhs->ptr, lhs->capacity);
    if (lhs->ptr == NULL)
      abort();
  }
  memcpy(((unsigned char*) lhs->ptr) + lhs->size, str, size);
  lhs->size += size;
}

/* Compare equal between @lhs and @rhs. */
inline int oak_buffer_compare(const oak_buffer* lhs,
                              const oak_buffer* rhs) {
  int r0 = memcmp(lhs->ptr, rhs->ptr, lhs->size);
  long int r1 = lhs->size - rhs->size;  // NOLINT
  if (r1 > INT_MAX) r1 = INT_MAX;
  else if (r1 < INT_MIN) r1 = INT_MIN;
  return r0 != 0 ? r0 : r1 != 0 ? r1 : 0;
}

/* struct oak_buffer_ref
 *
 * like as oak_buffer but it does not own the underlying data.
 * and it does not modify the underlying data. */

struct oak_buffer_ref {
  const void* ptr;
  size_t size;
};

#define OAK_MEMBER_SIZE(type, field) (sizeof(((type*) 0)->field))  // NOLINT

/* Create an referrence from the oak_buffer. */
inline struct oak_buffer_ref oak_as_buffer_ref(
    const struct oak_buffer* buffer) {
  assert(offsetof(struct oak_buffer, ptr) ==
         offsetof(struct oak_buffer_ref, ptr) &&
         OAK_MEMBER_SIZE(struct oak_buffer, ptr) ==
         OAK_MEMBER_SIZE(struct oak_buffer_ref, ptr));
  assert(offsetof(struct oak_buffer, size) ==
         offsetof(struct oak_buffer_ref, size) &&
         OAK_MEMBER_SIZE(struct oak_buffer, size) ==
         OAK_MEMBER_SIZE(struct oak_buffer_ref, size));

  return *(const struct oak_buffer_ref*) buffer;
}

/* Create an referrence from the c-style string. */
inline struct oak_buffer_ref
oak_as_buffer_ref_str(const char* str, size_t size) {
  const struct oak_buffer_ref piece = {
    .ptr = str,
    .size = size
  };
  return piece;
}

/* Compare equal between @lhs and @rhs. */
inline int oak_buffer_ref_compare(const oak_buffer_ref lhs,
                                  const oak_buffer_ref rhs) {
  int r0 = memcmp(lhs.ptr, rhs.ptr, lhs.size);
  long int r1 = lhs.size - rhs.size;  // NOLINT
  if (r1 > INT_MAX) r1 = INT_MAX;
  else if (r1 < INT_MIN) r1 = INT_MIN;
  return r0 != 0 ? r0 : r1 != 0 ? r1 : 0;
}

/* Test whether it is empty. */
inline int oak_buffer_ref_empty(const struct oak_buffer_ref piece) {
  return piece.size == 0;
}

#ifdef __cplusplus
}
#endif

#endif  /* OAK_ADDONS_PUBLIC_BUFFER_H_ */
