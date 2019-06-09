#include <iostream>

void func(int v) {
  std::cout << v << std::endl;
}

void func(bool v) {
  std::cout << v << std::endl;
}

int main() {
  func(10);
  return 0;
}

