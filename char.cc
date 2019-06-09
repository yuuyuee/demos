#include <iostream>

int main() {
  for (char c = 0x20; c <= 0x7e; ++c)
    std::cout << c;
  std::cout << std::endl;
  return 0;
}
