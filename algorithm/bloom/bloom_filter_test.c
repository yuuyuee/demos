/* Author: yuyue (X3130@njsecnet.com) */

#include "bloom_filter.h"

#include <stdio.h>

int main() {
  struct bloom_filter* bf = bloom_filter_alloc(80000000, 0.0001);
  if (!bf) {
    printf("bloom_filter_alloc failed\n");
    return -1;
  }

  bloom_filter_add(bf, "hello", 5);
  bloom_filter_add(bf, "world", 5);

  double got = 0.0;
  double total = 0.0;

  char key[] = {'\0', '\0'};
  for (char c = 'a'; c <= 'z'; ++c) {
    key[0] = c;
    if (bloom_filter_test(bf, key, 1)) {
      printf("%c already exists or collision\n", c);
      ++got;
    } else {
      printf("%c not exists\n", c);
    }
    ++total;
  }
  printf("Probability of false positive: %.2f\n", got / total);

  if (bloom_filter_test(bf, "world", 5)) {
    printf("world already exists\n");
  }

  bloom_filter_free(bf);
  return 0;
}
