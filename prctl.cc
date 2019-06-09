#include <iostream>
#include <sys/prctl.h>
#include <cstdlib>
#include <cstring>
#include <cerrno>

int main() {
    int ret = prctl(PR_SET_NAME, "[Thread 0]");
    if (ret) {
        std::cout << "prctl: " << strerror(errno) << std::endl;
        exit(0);
    }

    while (1);
    return 0;
}
