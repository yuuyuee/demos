#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cout << "getline <file>" << std::endl;
    return 0;
  }

  std::ifstream in(argv[1]);
  if (in) {
    std::string line;

    while (std::getline(in, line)) {
      std::cout << line << std::endl;
    }
  }

  return 0;
}
