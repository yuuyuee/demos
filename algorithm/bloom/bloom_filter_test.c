/* Author: yuyue (X3130@njsecnet.com) */

#include "bloom_filter.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void bloom_filter_visitor(uint64_t bytes, uint64_t entries, uint64_t hashs, double p) {
  printf("bytes = %lu entries = %lu hashs = %lu p = %f%%\n", bytes, entries, hashs, p);
}

int main(int argc, char* argv[]) {
  uint64_t n = 80000000;
  double p = 0.0001;

  if (argc == 3) {
    n = atol(argv[1]);
    p = atof(argv[2]);
  }

  struct bloom_filter* bf = bloom_filter_alloc(n, p);
  if (!bf) {
    printf("bloom_filter_alloc failed\n");
    return -1;
  }

  bloom_filter_insert(bf, "hello", 5);
  bloom_filter_insert(bf, "world", 5);

  assert(bloom_filter_lookup(bf, "world", 5) == 1);
  assert(bloom_filter_lookup(bf, "foo", 3) == 0);

  bloom_filter_visit(bf, bloom_filter_visitor);
  bloom_filter_free(bf);
  return 0;
}
