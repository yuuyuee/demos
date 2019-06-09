#include <iostream>
#include <string>
<<<<<<< HEAD
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
=======

using std::cout;
using std::endl;
using std::string;

int main(void) {
    char buffer[1024] = {0};
    string str(buffer);
    str.append("123");
    cout << buffer << endl;
    return 0;
>>>>>>> 41b0cdc026df773ac91fd7a50f6a4205cb0c7772
}
