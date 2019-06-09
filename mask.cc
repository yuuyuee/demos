#include <iostream>
#include <cstdio>
#include <cstdint>
#include <limits>

template<typename T, int bit = sizeof(T)>
struct mask {
  static const T value = bit <= 0 ? 1 : ((1 << (bit - 1)) | mask<T, bit - 1>::value);
};

int main() {
  printf("%x\n", mask<uint8_t>::value);
  printf("%x\n", std::numeric_limits<uint8_t>::max());
  printf("%x\n", std::numeric_limits<uint16_t>::max());
  printf("%x\n", std::numeric_limits<uint32_t>::max());
  printf("%lx\n", std::numeric_limits<uint64_t>::max());
  return 0;
}
