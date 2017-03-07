#include <iostream>

class test {
public:
    test() {
        std::cout << "constructor" << std::endl;
    }
    operator test() {
        class test t;
        std::cout << "call test()" << std::endl;
        return t;
    }

    void operator()() {
        std::cout << "call ()()" << std::endl;
    }
};

int main(void) {
    test();

    return 0;
}
