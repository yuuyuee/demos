#include <iostream>
#include <exception>
#include <cassert>

void ThrowLogicError() {
  throw std::logic_error("Test exception");
  if (std::uncaught_exception() == true)
    std::cout << "uncaught exception" << std::endl;
}

int main() {
  ThrowLogicError();
  return 0;
}
