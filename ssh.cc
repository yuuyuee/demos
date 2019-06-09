#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cerrno>
#include <cstring>

int main() {
  FILE* file = popen("scp root@172.16.24.84:/etc/passwd .", "w");
  if (file == NULL) {
    std::cout << strerror(errno) << std::endl;
    return -1;
  }
  const char* pass = "123456";
  fwrite(pass, sizeof(pass), 1, file);
  pclose(file);

  return 0;
}
