#include <iostream>
#include <sys/prctl.h>
#include <cstdlib>
#include <cstring>
#include <cerrno>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

void Handler(int sig) {
  std::cout << sig << " = " << (sig == SIGUSR1 ? "SIGUSR1" : "UNKNOWN") << std::endl;
  exit(1);
}

int main(int argc, char* argv[]) {
  pid_t pid = fork();
  if (pid < 0) {
    std::cout << "fork failed" << std::endl;
    exit(-1);
  } else if (pid == 0) {  // child process
    signal(SIGUSR1, Handler);
    //int ret = prctl(PR_SET_PDEATHSIG, SIGUSR1);
    //if (ret < 0) {
    //  std::cout << "prctl failed" << std::endl;
    //  exit(-1);
    //}

    std::cout << "child running" << std::endl;
    while (true)
      sleep(3);
    return 0;
  }

  sleep(1);
  std::cout << "parent try to exit" << std::endl;
  sleep(1);
  return 0;
}
