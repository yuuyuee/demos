#include <iostream>
#include <type_traits> // for std::alignment_of

struct test {
  int i;
  char c;
};

int main() {
  test normal_test;
  alignas(alignof(test)) test align_test;

  std::cout << "normal test: " << &normal_test << std::endl;
  std::cout << "align test: " << &align_test << std::endl;

  std::cout << "alignof(test): " << alignof(test) << std::endl;
  std::cout << "std::alignment_of<test>::value: " << std::alignment_of<test>::value << std::endl;
  return 0;
}
