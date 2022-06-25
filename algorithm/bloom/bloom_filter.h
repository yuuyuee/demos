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
   elementes before using and probability of false positive @p. */
struct bloom_filter* bloom_filter_alloc(uint64_t entries, double p);

/* Deallocate internal storage. */
void bloom_filter_free(struct bloom_filter* object);

/* Check if the given element is in the bloom filter.
   Returns:
    0 - element was not present.
    1 - indicates if element had already been added previously or a collision
        (probability of false positive). */
int bloom_filter_lookup_hash(const struct bloom_filter* object, uint64_t hashval);
int bloom_filter_lookup(const struct bloom_filter* object, const void* buffer, size_t len);

/* Add the given element to the bloom filter.*/
void bloom_filter_insert_hash(struct bloom_filter* object, uint64_t hashval);
void bloom_filter_insert(struct bloom_filter* object, const void* buffer, size_t len);

/* Check if the given element is in the bloom filter, that will be inserted if not present.
   Returns:
    0 - element was not present and was inserted.
    1 - indicates if element had already been added previously or a collision
        (probability of false positive). */
int bloom_filter_lookup_and_insert_hash(struct bloom_filter* object, uint64_t hashval);
int bloom_filter_lookup_and_insert(struct bloom_filter* object, const void* buffer, size_t len);


typedef void (*Visitor)(
   uint64_t /* bytes */, uint64_t /* entries */, uint64_t /* hashs */, double /* Pfp */);
void bloom_filter_visit(struct bloom_filter* object, Visitor visitor);

#ifdef __cplusplus
}
#endif

#endif
