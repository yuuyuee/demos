#include <ucontext.h>
#include <signal.h>

#include <iostream>
#include <thread>
#include <chrono>

void coroutine(ucontext_t* context) {
  std::cout << "[" << std::this_thread::get_id() << "] coroutine init" << std::endl;
  std::this_thread::sleep_for(std::chrono::seconds(1));
  swapcontext(&(context[1]), &(context[0]));
  std::cout << "[" << std::this_thread::get_id() << "] coroutine work" << std::endl;
}

int main() {
  char stack[SIGSTKSZ * 10];
  ucontext_t context[2];
  getcontext(&(context[1]));
  context[1].uc_stack.ss_sp = stack;
  context[1].uc_stack.ss_size = sizeof(stack);
  context[1].uc_link = nullptr;
  makecontext(&(context[1]), (void (*)()) coroutine, 1, context);
  swapcontext(&(context[0]), &(context[1]));
  std::cout << "[" << std::this_thread::get_id() << "] coroutine finish" << std::endl;
  return 0;
}
