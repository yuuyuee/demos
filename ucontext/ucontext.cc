#include <ucontext.h>
#include <signal.h>
#include <errno.h>

#include <iostream>
#include <thread>
#include <chrono>
#include <system_error>

static ucontext_t uctx_main, uctx_func1, uctx_func2;

static void func1() {
  std::cout << "[" << std::this_thread::get_id() << "] func1: started" << std::endl;
  std::cout << "[" << std::this_thread::get_id() << "] func1: swapcontext(&uctx_func1, &uctx_func2)" << std::endl;
  if (swapcontext(&uctx_func1, &uctx_func2) < 0)
    throw std::system_error(errno, std::system_category(), "swapcontext");
  std::cout << "[" << std::this_thread::get_id() << "] func1: returning" << std::endl;
}

static void func2() {
  std::cout << "[" << std::this_thread::get_id() << "] func2: started" << std::endl;
  std::cout << "[" << std::this_thread::get_id() << "] func2: swapcontext(&uctx_func2, &uctx_func1)" << std::endl;
  if (swapcontext(&uctx_func2, &uctx_func1) < 0)
    throw std::system_error(errno, std::system_category(), "swapcontext");
  std::cout << "[" << std::this_thread::get_id() << "] func2: returning" << std::endl;
}

int main() {
  char func1_stack[4 * 4096];
  char func2_stack[4 * 4096];

  if (getcontext(&uctx_func1) < 0)
    throw std::system_error(errno, std::system_category(), "getcontext");
  uctx_func1.uc_stack.ss_sp = func1_stack;
  uctx_func1.uc_stack.ss_size = sizeof(func1_stack);
  uctx_func1.uc_link = &uctx_main;
  makecontext(&uctx_func1, (void (*)()) func1, 0);

  if (getcontext(&uctx_func2) < 0)
    throw std::system_error(errno, std::system_category(), "getcontext");
  uctx_func2.uc_stack.ss_sp = func2_stack;
  uctx_func2.uc_stack.ss_size = sizeof(func2_stack);
  uctx_func2.uc_link = &uctx_func1;
  makecontext(&uctx_func2, (void (*)()) func2, 0);

  std::cout << "[" << std::this_thread::get_id() << "] main: swapcontext(&uctx_main, &uctx_func2)" << std::endl;
  if (swapcontext(&uctx_main, &uctx_func2) < 0)
    throw std::system_error(errno, std::system_category(), "swapcontext");
  std::cout << "[" << std::this_thread::get_id() << "] main: exiting" << std::endl;
  return 0;
}
