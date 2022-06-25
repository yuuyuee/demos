/* Author: yuyue (X3130@njsecnet.com) */

#include "bloom_filter.h"

#include <stdio.h>
#include <stdlib.h>

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

  double got = 0.0;
  double total = 0.0;

  char key[] = {'\0', '\0'};
  for (char c = 'a'; c <= 'z'; ++c) {
    key[0] = c;
    if (bloom_filter_lookup(bf, key, 1)) {
      printf("%c already exists or collision\n", c);
      ++got;
    } else {
      printf("%c not exists\n", c);
    }
    ++total;
  }
  printf("Probability of false positive: %f%%\n", got / total * 100);

  if (bloom_filter_lookup(bf, "world", 5)) {
    printf("world already exists\n");
  }

  bloom_filter_visit(bf, bloom_filter_visitor);
  bloom_filter_free(bf);
  return 0;
}
