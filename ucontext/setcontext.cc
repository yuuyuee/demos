#include <ucontext.h>

#include <iostream>
#include <thread>
#include <chrono>

int main() {
  ucontext_t context;
  getcontext(&context);
  std::cout << "[" << std::this_thread::get_id() << "] hello world" << std::endl;
  std::this_thread::sleep_for(std::chrono::seconds(1));
  setcontext(&context);
  return 0;
}