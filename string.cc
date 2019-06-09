#include <iostream>
#include <string>
#include <cstring>

using std::string;

int main(void) {
  std::string s;
  int i = 100;
  s = std::to_string(i);
  std::cout << s << std::endl;
#if 0
  const char* str = "123abc";
  i = std::stoi(str);
  std::cout << i << std::endl;
#endif
  return 0;
}
