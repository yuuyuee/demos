#include <iostream>
#include <stddef.h>
#include <type_traits> // for std::alignment_of

struct alignas(64) test {
  int i;
  char c;
};

struct alignas(64) test2 {
  alignas(64) int i;
  alignas(64) char c;
  alignas(64) test t;
};

int main() {
  test normal_test;
  alignas(alignof(test)) test align_test;
  test2 test22;

  std::cout << "normal test: " << &normal_test << std::endl;
  std::cout << "align test: " << &align_test << std::endl;
  std::cout << "align &test.i " << (offsetof(test, i)) << std::endl;
  std::cout << "align &test.c " << (offsetof(test, c)) << std::endl;

  std::cout << "align test2: " << &test22 << std::endl;
  std::cout << "align &test2.i " << (offsetof(test2, i)) << std::endl;
  std::cout << "align &test2.c " << (offsetof(test2, c)) << std::endl;

  std::cout << "sizeof(test): " << sizeof(test) << std::endl;
  std::cout << "alignof(test): " << alignof(test) << std::endl;
  std::cout << "std::alignment_of<test>::value: " << std::alignment_of<test>::value << std::endl;

  test2* ptr = new test2;
  delete ptr;

  return 0;
}
