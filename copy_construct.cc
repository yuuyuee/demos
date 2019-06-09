#include <iostream>
#include <initializer_list>

class test {
public:
    test() {
        std::cout << "Default construct Function" << std::endl;
    }
    test(const test& t) {
        std::cout << "Copy construct Function" << std::endl;
    }

    test& operator=(const test& t) {
        std::cout << "Assignment construct Function" << std::endl;
        return *this;
    }
};

int main(void) {
    test a; // default
    test b(a); // copy
    test c; // default
    c = a; // assignment
    test __attribute__((unused)) d = a; // copy
    return 0;
}
