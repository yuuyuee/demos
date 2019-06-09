#include <iostream>

struct test {
    test() {
        std::cout << "constructor" << std::endl;
    }

    void operator()() {
        std::cout << "call ()()" << std::endl;
    }
};

int main(void) {
    test();
    test()();
    test{}();

    return 0;
}
