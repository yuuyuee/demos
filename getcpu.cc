#include <iostream>
#include <unistd.h>

int main(void) {
    std::cout << sysconf(_SC_NPROCESSORS_CONF) << std::endl;
    return 0;
}
