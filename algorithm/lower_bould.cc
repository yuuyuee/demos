#include <iostream>

static int LowerBound(const int* ptr, size_t n, int val) {
    while (n > 0) {
        size_t half = n / 2;
        if (*(ptr + half) < val) {
            ptr += half + 1;
            n -= half + 1;
        } else {
            n = half;
        }
    }
    return *ptr;
}

int main() {
    int a[] = {0, 1, 2, 3, 4, 6, 7, 7};
    std::cout << LowerBound(a, sizeof(a) / sizeof(*a) - 1, 5) << std::endl;
    std::cout << LowerBound(a, sizeof(a) / sizeof(*a) - 1, 100) << std::endl;
    return 0;
}
