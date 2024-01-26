#include <iostream>

extern "C" {
void fn_b() {
  std::cout << "fn_b" << std::endl;
}
}