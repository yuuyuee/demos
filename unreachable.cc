#include <iostream>

void func() {
  __builtin_unreachable();
}

int main() {
  func();
  return 0;
}
