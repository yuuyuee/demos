#include <iostream>
<<<<<<< HEAD
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
=======
#include <string>
#include <exception>

// class exception
//      class logic_error: public exception
//          class domain_error: public logic_error
//          class invalid_argument: public logic_error
//          class length_error: public logic_error
//          class out_of_range: public logic_error
//      class rumtime_error: public exception
//          class range_error: public runtime_error
//          class overflow_error: public runtime_error
//          class underflow_error: public runtime_error

template<typename Tp>
void What(const std::string& title, const Tp& e) {
    std::cout << title << ": " << e.what() << std::endl;
}

int main(void) {
    What("std::exception", std::exception());

    What("\tstd::logic_error", std::logic_error("std::logic_error"));
    What("\t\tstd::domain_error", std::domain_error("std::domain_error"));
    What("\t\tstd::invalid_error", std::invalid_argument("std::invalid_argument"));
    What("\t\tstd::length_error", std::length_error("std::invalid_argument"));
    What("\t\tstd::out_of_range", std::out_of_range("std::out_of_range"));

    What("\tstd::runtime_error", std::runtime_error("std::runtime_error"));
    What("\t\tstd::range_error", std::range_error("std::range_error"));
    What("\t\tstd::overflow_error", std::overflow_error("std::overflow_error"));
    What("\t\tstd::underflow_error", std::underflow_error("std::underflow_error"));

    int num = std::uncaught_exceptions();
    std::cout << num << std::endl;

    return 0;
>>>>>>> 41b0cdc026df773ac91fd7a50f6a4205cb0c7772
}
