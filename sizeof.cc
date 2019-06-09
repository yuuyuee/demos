#include <iostream>
#include <cstdint>

#define DUMPSIZE(type) \
  std::cout << "sizeof(" #type ") = " << sizeof(type) << std::endl

int main() {
  DUMPSIZE(char);
  DUMPSIZE(short);
  DUMPSIZE(int);
  DUMPSIZE(long int);
  DUMPSIZE(long long int);
  return 0;
}
