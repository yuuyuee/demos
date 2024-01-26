#include <iostream>

extern "C" {
void fn_b();

void fn_a() {
  std::cout << "fn_a" << std::endl;
  fn_b();
}
}