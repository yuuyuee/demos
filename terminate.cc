#include <csignal>
#include <iostream>
#include <stdexcept>

void handler(int sig) {
  std::cout << "caught signal: " << sig << std::endl;
}

void throw_except() {
  throw std::logic_error("xxx");
}

int main() {
  signal(SIGABRT, handler);
  throw_except();
  return 0;
}
