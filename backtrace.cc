#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <cxxabi.h>

void myfunc3(void) {
int j, nptrs;
#define SIZE 100
void *buffer[100];
char **strings;

nptrs = backtrace(buffer, SIZE);
printf("backtrace() returned %d addresses\n", nptrs);

/* The call backtrace_symbols_fd(buffer, nptrs, STDOUT_FILENO)
  would produce similar output to the following: */
  strings = backtrace_symbols(buffer, nptrs);
  if (strings == NULL) {
    perror("backtrace_symbols");
    exit(EXIT_FAILURE);
  }

  for (j = 0; j < nptrs; j++) {
    printf("%s", strings[j]);
    const std::string pc(strings[j]);
    auto start = pc.find('(');
    if (start != std::string::npos && start + 1 < pc.size())
      start++;
    auto end = pc.find('+');
    if (end != std::string::npos && end > start) {
      const std::string symbol(pc.substr(start, end - start));
      int status = 0;
      size_t size = 100;
      char buf[size];
      auto ret = abi::__cxa_demangle(symbol.data(), buf, &size, &status);
      if (ret) {
        printf(" - %s", buf);
      } else {
        printf(" - (unknown)");
      }
    }
    printf("\n");
  }

  free(strings);
}

std::string myfunc2(void) {
 myfunc3();
 return {};
}

void myfunc() {
  myfunc2();
}

int main() {
  myfunc();
  exit(EXIT_SUCCESS);
}

