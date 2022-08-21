#include <iostream>
#include <new>

int main() {
//#ifdef __cpp_lib_hardare_interference_size
  std::cout << std::hardware_constructive_interference_size << std::endl;
  std::cout << std::hardware_destructive_interference_size << std::endl;
//#endif
  return 0;
}
