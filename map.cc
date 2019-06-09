#include <iostream>
#include <map>
#include <string>

int main() {
  std::map<std::string, std::string> m1, m2;
  std::pair<std::string, std::string> item1 = {"1", "1"};
  std::pair<std::string, std::string> item2 = {"2", "2"};
  std::pair<std::string, std::string> item3 = {"3", "3"};
  m1.insert(item1);
  m1.insert(item2);
  m2.insert(item3);
  auto m3 = m1 + m2;
  return 0;
}
