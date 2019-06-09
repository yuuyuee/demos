#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstring>
#include <cerrno>

int main() {
    std::cout << strerror(-ENOMEM) << std::endl;
    return 0;
}
