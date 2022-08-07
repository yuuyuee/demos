#include <iostream>
#include <csignal>
#include <thread>
#include <chrono>

void func() {
  for (;;) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "function in func" << std::endl;
  }
}

void sig(int signo) {
  std::this_thread::sleep_for(std::chrono::seconds(1));
  std::cout << "function in sig" << std::endl;
  signal(signo, SIG_DFL);
  raise(signo);
}

int main() {
  signal(SIGINT, sig);
  std::thread td(func);
  while (1) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "function in main" << std::endl;
  }
  return 0;
}
