#include <iostream>

namespace c {
int value_in_c = 10;

void fn_c() {
  std::cout << "fn_c: " << fn_c << std::endl;
  std::cout << "&value_in_c: " << &value_in_c << std::endl;
}
}