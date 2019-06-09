#include <iostream>
#include <string>
#include <utility>

void print_lvalue(const int& v) {
    std::cout << "lvalue: " << v << std::endl;
}

void print_rvalue(int&& v) {
    std::cout << "rvalue: " << v << std::endl;
}

int main(void) {
    int v = 10;

    print_lvalue(v);
    print_rvalue(v + 1);

    return 0;
}
