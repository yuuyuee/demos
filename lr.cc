#include <iostream>
#include <string>
#include <utility>

<<<<<<< HEAD
void print_lvalue(const int& v) {
    std::cout << "lvalue: " << v << std::endl;
}

void print_rvalue(int&& v) {
    std::cout << "rvalue: " << v << std::endl;
}

=======
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

>>>>>>> 41b0cdc026df773ac91fd7a50f6a4205cb0c7772
int main(void) {
    int v = 10;

    print_lvalue(v);
<<<<<<< HEAD
    print_rvalue(v + 1);
=======
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
>>>>>>> 41b0cdc026df773ac91fd7a50f6a4205cb0c7772

    return 0;
}
