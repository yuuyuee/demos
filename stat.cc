#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <cstring>
#include <cerrno>

int main() {
  struct stat st;
  stat("xxx", &st);
  std::cout << errno << ": " << strerror(errno) << std::endl;
  return 0;
}
