#include <iostream>

static void print_ref_adress(int &v) {
    std::cout << "In function Pointer: " << &v << std::endl;
}

int main(void) {
    int v = 100;
    std::cout << "Pointer: " << &v << std::endl;
    print_ref_adress(v);

    return 0;
}
