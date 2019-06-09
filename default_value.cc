#include <iostream>
#include <typeindex>

#define MAKE(xxx)           \
    xxx(char)               \
    xxx(short)              \
    xxx(int)                \
    xxx(long)               \
    xxx(float)              \
    xxx(double)

#define xxx(type) \
    std::cout << #type <<  " default value: " << type() << std::endl;

int main(void) {
    MAKE(xxx)
    return 0;
}
