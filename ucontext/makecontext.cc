#include <ucontext.h>
#include <signal.h>

#include <iostream>
#include <thread>
#include <chrono>

void coroutine(const ucontext_t* context) {
  std::cout << "[" << std::this_thread::get_id() << "] hello world" << std::endl;
  std::this_thread::sleep_for(std::chrono::seconds(1));
  setcontext(context);
}

int main() {
  char stack[SIGSTKSZ * 10];
  ucontext_t context;
  getcontext(&context);
  context.uc_stack.ss_sp = stack;
  context.uc_stack.ss_size = sizeof(stack);
  context.uc_link = nullptr;
  makecontext(&context, (void (*)()) coroutine, 1, &context);
  setcontext(&context);
  return 0;
}
