/* Author: yuyue (X3130@njsecnet.com) */

#ifndef BLOOM_FILTER_H_
#define BLOOM_FILTER_H_

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

uint64_t bloom_filter_hash(const void* buffer, size_t len);

struct bloom_filter;

/* Initialize the bloom filter with given @entries of the number of inserted
   elementes before using and probability of false positive @pfp. */
struct bloom_filter* bloom_filter_alloc(uint64_t entries, double pfp);

/* Deallocate internal storage. */
void bloom_filter_free(struct bloom_filter* object);

/* Check if the given element is in the bloom filter.
   Returns:
    0 - element was not present.
    1 - indicates if element had already been added previously or a collision
        (probability of false positive). */
int bloom_filter_test_hash(struct bloom_filter* object, size_t hashval);
int bloom_filter_test(struct bloom_filter* object, const void* buffer, size_t len);

/* Add the given element to the bloom filter.*/
void bloom_filter_add_hash(struct bloom_filter* object, size_t hashval);
void bloom_filter_add(struct bloom_filter* object, const void* buffer, size_t len);

typedef void (*Visitor)(uint64_t m, uint64_t n, uint64_t k, double pfp);
void bloom_filter_visit(struct bloom_filter* object, Visitor visitor);

#ifdef __cplusplus
}
#endif

#endif
