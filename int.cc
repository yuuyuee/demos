#include <iostream>
#include <limits>
#include <stdint.h>

#define COUT(x) std::cout << #x << " = " << (x) << std::endl

int main() {
  uint32_t max = 1 << 10;
  uint32_t mask = max - 1;
  uint32_t head = 1;
  uint32_t tail = 1000;

  COUT(max);
  COUT(mask);

  COUT(head);
  COUT(tail);

  COUT(tail - head);
  COUT(mask + tail - head);
  COUT((tail - head) & (mask));

  COUT(head - tail);
  COUT(mask + head - tail);
  COUT((head - tail) & (mask));

  return 0;
}
