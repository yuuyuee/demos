/* Author: yuyue (X3130@njsecnet.com) */

#include "bloom_filter.h"

#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <assert.h>

#define FALLTHROUGH_TAG_IMPL(tag)         \
  goto _FALLTHROUGHT_TAG_ ## tag; break;  \
  _FALLTHROUGHT_TAG_ ## tag:

#define FALLTHROUGH_TAG(tag) FALLTHROUGH_TAG_IMPL(tag)
#define FALLTHROUGH FALLTHROUGH_TAG(__LINE__)

// MurmurHash64A, 64-bit versions, by Austin Appleby
static uint64_t MurmurHash64A(const void* buffer, size_t len, uint64_t seed) {
  const uint64_t m = 0xc6a4a7935bd1e995ULL;
  const int r = 47;
  uint64_t h = seed ^ (len * m);
  const uint64_t *ptr = (const uint64_t *)buffer;
  const uint64_t *end = ptr + (len / 8);

  while (ptr != end) {
    uint64_t k = *ptr++;

    k *= m;
    k ^= k >> r;
    k *= m;

    h ^= k;
    h *= m;
  }

  switch (len % 8) {
  case 7:
    h ^= ((uint64_t)ptr[6]) << 48;
    // Many compilers and analysis tools will flag fall through as a warning.
    FALLTHROUGH;
  case 6:
    h ^= ((uint64_t)ptr[5]) << 40;
    FALLTHROUGH;
  case 5:
    h ^= ((uint64_t)ptr[4]) << 32;
    FALLTHROUGH;
  case 4:
    h ^= ((uint64_t)ptr[3]) << 24;
    FALLTHROUGH;
  case 3:
    h ^= ((uint64_t)ptr[2]) << 16;
    FALLTHROUGH;
  case 2:
    h ^= ((uint64_t)ptr[1]) << 8;
    FALLTHROUGH;
  case 1:
    h ^= ((uint64_t)ptr[0]);
    h *= m;
  }

  h ^= h >> r;
  h *= m;
  h ^= h >> r;

  return h;
}

uint64_t bloom_filter_hash(const void* buffer, size_t len) {
  return MurmurHash64A(buffer, len, 0xc6a4a7935bd1e995ULL);
}

struct bloom_filter {
  double p;
  double bpe;
  uint64_t n;
  uint64_t m;
  uint64_t k;

  uint64_t bytes;
  unsigned char* array;
};

struct bloom_filter* bloom_filter_alloc(uint64_t entries, double p) {
  if (p <= 0 || p >= 1.0 || entries == 0)
    return NULL;

  struct bloom_filter* bf = malloc(sizeof(*bf));
  if (!bf)
    goto alloc_failed;

  bf->p = p;
  bf->bpe = fabs(-(log(p) / 0.480453013918201));  /* ln(2)^2 */
  bf->n = entries;
  bf->m = (uint64_t)ceil(entries * bf->bpe);
  bf->k = (uint64_t)ceil(bf->bpe * 0.693147180559945);  /* ln(2) */
  bf->k = bf->k > 30 ? 30 : bf->k;
  bf->bytes = bf->m + 7 / 8;
  bf->bytes = bf->bytes < 8 ? 8 : bf->bytes;
  bf->m = bf->bytes * 8;

  bf->array = (unsigned char*)calloc(bf->bytes, 1);
  if (!bf->array)
    goto alloc_array_failed;
  return bf;

alloc_array_failed:
  free(bf);

alloc_failed:
  return NULL;
}

void bloom_filter_free(struct bloom_filter* object) {
  free(object->array);
  free(object);
}

int bloom_filter_lookup_hash(const struct bloom_filter* object, uint64_t hashval) {
  const uint64_t delta = (hashval << 15) | (hashval >> 49);
  for (uint64_t i = 0; i < object->k; ++i) {
    const uint64_t pos = hashval % object->m;
    if ((object->array[pos / 8] & (1 << (pos % 8))) == 0)
      return 0;
    hashval += delta;
  }
  return 1;
}

int bloom_filter_lookup(const struct bloom_filter* object, const void* buffer, size_t len) {
  uint64_t hashval = bloom_filter_hash(buffer, len);
  return bloom_filter_lookup_hash(object, hashval);
}

void bloom_filter_insert_hash(struct bloom_filter* object, uint64_t hashval) {
  const uint64_t delta = (hashval << 15) | (hashval >> 49);
  for (uint64_t i = 0; i < object->k; ++i) {
    const uint64_t pos = hashval % object->m;
    object->array[pos / 8] |= (1 << (pos % 8));
    hashval += delta;
  }
}

void bloom_filter_insert(struct bloom_filter* object, const void* buffer, size_t len) {
  uint64_t hashval = bloom_filter_hash(buffer, len);
  bloom_filter_insert_hash(object, hashval);
}

int bloom_filter_lookup_and_insert_hash(struct bloom_filter* object, uint64_t hashval) {
  int flag = 1;
  const uint64_t delta = (hashval << 15) | (hashval >> 49);
  for (uint64_t i = 0; i < object->k; ++i) {
    const uint64_t pos = hashval % object->m;
    if ((object->array[pos / 8] & (1 << (pos % 8))) == 0) {
      flag =  0;
      object->array[pos / 8] |= (1 << (pos % 8));
    }
    hashval += delta;
  }
  return flag;
}

int bloom_filter_lookup_and_insert(struct bloom_filter* object, const void* buffer, size_t len) {
  uint64_t hashval = bloom_filter_hash(buffer, len);
  return bloom_filter_lookup_and_insert_hash(object, hashval);
}

void bloom_filter_visit(struct bloom_filter* object, Visitor visitor) {
  visitor(object->bytes, object->n, object->k, object->p);
}
