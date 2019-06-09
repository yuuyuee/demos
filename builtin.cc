#include <iostream>
#include <limits>

int main() {
  unsigned int a = 0, b = 1, c = std::numeric_limits<unsigned int>::max();
  std::cout << 0 - __builtin_clz(a) << std::endl;
  std::cout << __builtin_clz(b) << std::endl;
  std::cout << __builtin_clz(c) << std::endl;
  return 0;
}
