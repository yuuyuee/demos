#include <iostream>
#include <stdexcept>
#include <cxxabi.h>
#include <csignal>
#include <unistd.h>
#include <cstring>
#include <thread>

void thread_fn() {
  std::cout << "New thread begin" << std::endl;
  raise(SIGABRT);
  std::cout << "New thread end" << std::endl;
}

void handler(int signo, siginfo_t*, void*) {
  std::cout << "0. caught signal " << signo << std::endl;
  static int i = 1;
  //signal(signo, SIG_DFL);
  if (!i) {
    ++i;
    raise(signo);
  }
  std::cout << "1. caught signal " << signo << std::endl;
}

int main(void) {
#if 0
  struct sigaction act;
  sigemptyset(&act.sa_mask);
  act.sa_flags |= SA_SIGINFO;
  act.sa_flags |= SA_NODEFER;
  act.sa_sigaction = handler;
  sigaction(SIGABRT, &act, nullptr);
#endif
  // __EXCEPTIONS __GNUC__ < 5
  // __cpp_exceptions __GNUC__ >= 5
  //throw std::logic_error("logic error");
  //raise(SIGABRT);
  //__builtin_abort();
  std::thread new_thread(thread_fn);
  new_thread.join();
  sleep(2);
  std::cout << "Main thread end" << std::endl;

  return 0;
}
