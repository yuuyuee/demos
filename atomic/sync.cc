#include <iostream>
#include <atomic>

int main(void) {
  std::atomic<int> b(10);
  int expect = 100;
  int new_val = 1000;

  std::cout << "atomic: " << b << std::endl;
  std::cout << "expected: " << expect << std::endl;
  std::cout << "------------------------------" << std::endl;
  auto ret = b.compare_exchange_strong(expect, new_val);
  std::cout << "return: " << ret << std::endl;
  std::cout << "atomic: " << b << std::endl;
  std::cout << "expected: " << expect << std::endl;
  std::cout << "new value: " << new_val << std::endl;

  return 0;
}
