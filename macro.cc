#include <iostream>

#define CONCAT_(a, b) a ## b
#define CONCAT(a, b) CONCAT_(a, b)

#define MAJOR 1
#define MINOR 1

int main(void) {
    //std::cout << CONCAT_(MAJOR, MINOR) << std::endl;
    std::cout << CONCAT(MAJOR, MINOR) << std::endl;
    return 0;
}
