#include <iostream>

int main(void) {
    static_assert(1, "zero");
    static_assert(0, "zero");

    return 0;
}
