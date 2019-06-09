#include <iostream>
#include <thread>
#include <chrono>
#include <ctime>

int main() {
  while (true) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "loop: " << time(0) << std::endl;
  }
  return 0;
}
