// Author: yuyue (X3130@njsecnet.com)

#include <iostream>

#define CHECK_MACRO(MACRO)                              \
    if (MACRO) {                                        \
        std::cout << "Defined " #MACRO << std::endl;    \
    }

int main() {
    CHECK_MACRO(_GNU_SOURCE)
    CHECK_MACRO(_LP64)
    return 0;
}
