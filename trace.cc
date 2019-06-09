#include <iostream>
#include <sys/ptrace.h>

int main(void) {
    int res = ptrace(PTRACE_TRACEME, 0, 0, 0);
    if (res < 0) {
        std::cout << "Do not allowed trace" << std::endl;
        exit(-1);
    }
    while (1);
    return 0;
}
