#include <iostream>
template<typename Tp>
void print_lvalue(const Tp& v) {
    std::cout << "lvalue: " << v << std::endl;
}

template<typename Tp>
void print_rvalue(Tp&& v) {
    std::cout << "rvalue: " << v << std::endl;
}

int main(void) {
    int v = 10;

    print_lvalue(v);
    print_rvalue(v);
    print_lvalue(10);
    print_rvalue(10);

    return 0;
}
