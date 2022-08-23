/* Copyright 2022 The Oak Authors. */

#include "oak/addons/buffer_internal.h"

#include <assert.h>
#include <limits.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Initialize the object.
 * Note: which @buffer object must be an uninitialized object. */
void oak_buffer_init(struct oak_buffer* buffer, size_t capacity) {
  buffer->size = 0;
  buffer->flags = OAK_BUFFER_OWNED;
  buffer->capacity = capacity;
  buffer->ptr = NULL;
  if (buffer->capacity > 0) {
    buffer->ptr = malloc(buffer->capacity + 1);
    if (!buffer->ptr)
      abort();
  }
}

/* Initialize the object with @len bytes string @str, but does not
 * owned the underlying data.
 * Note: which @buffer object must be an uninitialized object. */
void oak_buffer_init2(struct oak_buffer* buffer,
                      const void* str, size_t len) {
  buffer->size = len;
  buffer->flags = 0;
  buffer->capacity = len;
  buffer->ptr = (void*) str;
}

/* Destruct object. */
void oak_buffer_free(struct oak_buffer* buffer) {
  if (buffer->flags & OAK_BUFFER_OWNED)
    free(buffer->ptr);
  buffer->flags = 0;
  buffer->ptr = NULL;
  buffer->size = 0;
  buffer->capacity = 0;
}

/* Move buffer ownership of @rhs to @lhs. */
void oak_buffer_move(struct oak_buffer* lhs, struct oak_buffer* rhs) {
  oak_buffer_free(lhs);
  *lhs = *rhs;
  rhs->flags &= ~OAK_BUFFER_OWNED;
  oak_buffer_free(rhs);
}

/* Copy buffer @rhs to @lhs. */
void oak_buffer_assign(struct oak_buffer* lhs, const struct oak_buffer* rhs) {
  assert((lhs->flags & OAK_BUFFER_OWNED) && "Require owned oak_buffer");
  if (lhs->capacity < rhs->size) {
    lhs->capacity = rhs->size;
    lhs->ptr = realloc(lhs->ptr, lhs->capacity + 1);
    if (!lhs->ptr)
      abort();
  }
  memcpy(lhs->ptr, rhs->ptr, rhs->size);
  lhs->size = rhs->size;
  ((unsigned char *) lhs->ptr)[rhs->size] = '\0';
}

/* Append buffer @rhs to @rhs. */
void oak_buffer_append(struct oak_buffer* lhs, const struct oak_buffer* rhs) {
  assert((lhs->flags & OAK_BUFFER_OWNED) && "Require owned oak_buffer");
  if (lhs->capacity < lhs->size + rhs->size) {
    lhs->capacity = lhs->size + rhs->size;
    lhs->capacity = lhs->capacity + lhs->capacity / 2;
    lhs->ptr = realloc(lhs->ptr, lhs->capacity);
    if (lhs->ptr == NULL)
      abort();
  }
  memcpy(((unsigned char*) lhs->ptr) + lhs->size, rhs->ptr, rhs->size);
  lhs->size += rhs->size;
  ((unsigned char *) lhs->ptr)[lhs->size] = '\0';
}

/* Append c-style string @str with @size bytes to buffer @lhs. */
void oak_buffer_append_str(struct oak_buffer* lhs,
                           const void* str, size_t size) {
  assert((lhs->flags & OAK_BUFFER_OWNED) && "Require owned oak_buffer");
  if (lhs->capacity < lhs->size + size) {
    lhs->capacity = lhs->size + size;
    lhs->capacity = lhs->capacity + lhs->capacity / 2;
    lhs->ptr = realloc(lhs->ptr, lhs->capacity);
    if (lhs->ptr == NULL)
      abort();
  }
  memcpy(((unsigned char*) lhs->ptr) + lhs->size, str, size);
  lhs->size += size;
  ((unsigned char *) lhs->ptr)[lhs->size] = '\0';
}

/* Compare equal between @lhs and @rhs. */
int oak_buffer_compare(const oak_buffer* lhs, const oak_buffer* rhs) {
  size_t min_size = lhs->size < rhs->size ? lhs->size : rhs->size;
  int r0 = memcmp(lhs->ptr, rhs->ptr, min_size);
  long int r1 = lhs->size - rhs->size;  // NOLINT
  if (r1 > INT_MAX) r1 = INT_MAX;
  else if (r1 < INT_MIN) r1 = INT_MIN;
  return r0 != 0 ? r0 : r1 != 0 ? r1 : 0;
}

/* Compare equal between @lhs and string @str with @size bytes. */
int oak_buffer_compare_str(const oak_buffer* lhs,
                           const void* str, size_t size) {
  size_t min_size = lhs->size < size ? lhs->size : size;
  int r0 = memcmp(lhs->ptr, str, min_size);
  long int r1 = lhs->size - size;  // NOLINT
  if (r1 > INT_MAX) r1 = INT_MAX;
  else if (r1 < INT_MIN) r1 = INT_MIN;
  return r0 != 0 ? r0 : r1 != 0 ? r1 : 0;
}

#ifdef __cplusplus
}
#endif
