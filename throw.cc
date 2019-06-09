#include <iostream>
#include <stdexcept>

void func() {
  std::cout << "before throw exception" << std::endl;
  throw std::exception();
  std::cout << "after throw exception" << std::endl;
}

int main() {
  try {
    func();
  } catch(...) {
    std::cout << "catch an exception" << std::endl;
  }
  return 0;
}
