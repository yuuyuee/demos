#include <iostream>
#include <cstring>

int main() {
  const char* s = "1abcdefg";
  const char* accept = "abc";
  const char* reject = "de";
  std::cout << strspn(s, accept) << std::endl;
  std::cout << strcspn(s, reject) << std::endl;
  return 0;
}
