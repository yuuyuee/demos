#include <iostream>
#include <string>
#include <utility>

template<typename Tp>
void print_lvalue(const Tp& v) {
    std::cout << "lvalue: " << v << std::endl;
}

template<typename Tp>
void print_rvalue(Tp&& v) {
    std::cout << "rvalue: " << v << std::endl;
}

void test_rvalue(std::string&& s) {
    std::cout << s << std::endl;
}

int main(void) {
    int v = 10;

    print_lvalue(v);
    print_rvalue(v);
    print_lvalue(10);
    print_rvalue(10);

    std::string s("hello world");
    test_rvalue(std::move(s));
    if (s.empty()) {
        std::cout << "s is empty" << std::endl;
    } else {
        std::cout << s << std::endl;
    }

    return 0;
}
