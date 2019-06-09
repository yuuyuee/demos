#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <csignal>
#include <cerrno>
#include <string>
#include <system_error>
#include <iostream>

static const char* named_pipe = "named_pipe";

int main() {
  signal(SIGPIPE, SIG_IGN);
  int fd = open(named_pipe, O_WRONLY);
  if (fd < 0) {
    std::cout << "open: " << std::make_error_code(std::errc(errno)).message() << std::endl;
    return -1;
  }

  while (1) {
    std::string buf;
    std::cin >> buf;
    auto size = write(fd, buf.data(), buf.size());
    if (size < 0) {
      std::cout << "write: " <<  std::make_error_code(std::errc(errno)).message() << std::endl;
      return -1;
    }
  }
  return 0;
}
