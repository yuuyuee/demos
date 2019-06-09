#include <iostream>
#include <string>
#include <cstdio>

int main() {
  char buffer[10];
  int len = snprintf(buffer, 10, "123");
  (void)len;
  std::cout << len << std::endl;
  std::cout << std::string(buffer) << std::endl;
  return 0;
}
