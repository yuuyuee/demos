#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <cerrno>
#include <string>
#include <system_error>
#include <iostream>

static const char* named_pipe = "named_pipe";

int main() {
  if (mkfifo(named_pipe, S_IWUSR | S_IRUSR) < 0 && errno != EEXIST) {
    std::cout << "mkfifo: " << std::make_error_code(std::errc(errno)).message() << std::endl;
    return -1;
  }

  int fd = open(named_pipe, O_RDONLY);
  if (fd < 0) {
    std::cout << "open: " << std::make_error_code(std::errc(errno)).message() << std::endl;
    return -1;
  }
  while (1) {
    static const size_t kMaxSize = 1024;
    char buf[kMaxSize];
    auto size = read(fd, buf, kMaxSize);
    if (size > 0) {
      std::cout << std::string(buf, size) << std::endl;
    } else {
      std::cout << "read: " << std::make_error_code(std::errc(errno)).message() << std::endl;
      return -1;
    }
  }
  close(fd);
  unlink(named_pipe);
  return 0;
}
