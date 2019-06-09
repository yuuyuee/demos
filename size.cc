#include <iostream>
#include <limits>

int main() {
  std::cout << std::numeric_limits<size_t>::max() << std::endl;
  std::cout << (3UL * 1024 * 1024 * 1024) * 8 / 10 << std::endl;
  return 0;
}
