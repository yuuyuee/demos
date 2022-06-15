#include <iostream>
#include <chrono>
#include <cstdlib>

int main() {
  for (int i = 0; i < 10; i++) {
    auto now = std::chrono::system_clock::now().time_since_epoch();
    auto sec = std::chrono::duration_cast<std::chrono::seconds>(now);
    auto nsec = std::chrono::duration_cast<std::chrono::nanoseconds>(now);
    std::cout << sec.count() << "." << nsec.count() << std::endl;
  }
  return 0;
}
