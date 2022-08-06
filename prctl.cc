#include <iostream>
#include <sys/prctl.h>
#include <cstdlib>
#include <cstring>
#include <cerrno>
#include <pthread.h>

int main(int argc, char* argv[]) {
  const char* process_name = "Process 0";
    int ret = prctl(PR_SET_NAME, process_name);
    if (ret) {
        std::cout << "prctl: " << strerror(errno) << std::endl;
        exit(0);
    }

    size_t len = strlen(argv[0]);
    memcpy(argv[0], process_name, len);

    char thread_name[] = "[Thread 0]";
    ret = pthread_getname_np(pthread_self(), thread_name, strlen(thread_name));
    if (ret < 0) {
      std::cout << "pthread_getname_np failed" << std::endl;
      exit(0);
    }

    while (1);
    return 0;
}
