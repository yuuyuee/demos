#include <iostream>
#include <atomic>
#include <memory>

int main(void) {
  std::atomic<bool> b;
  bool expected = false;
  bool ret = false;

  while (!(ret = b.compare_exchange_weak(expected, true)) && !expected)
    ;
  std::cout << "atomic: " << b << std::endl;
  std::cout << "expected: " << expected << std::endl;
  std::cout << "return: " << ret << std::endl;

  return 0;
}
