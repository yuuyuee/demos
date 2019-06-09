#include <iostream>
#include <cstdlib>
#include <cerrno>

int main() {
  std::cout << strtoll("0x7", 0, 10) << ", errno = " << errno << std::endl;
  std::cout << strtoll(0, 0, 10) << ", errno = " << errno << std::endl;
  return 0;
}
