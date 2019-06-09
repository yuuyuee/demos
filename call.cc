#include <iostream>

<<<<<<< HEAD
struct test {
    test() {
        std::cout << "constructor" << std::endl;
    }
=======
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
>>>>>>> 41b0cdc026df773ac91fd7a50f6a4205cb0c7772

    void operator()() {
        std::cout << "call ()()" << std::endl;
    }
};

int main(void) {
    test();
<<<<<<< HEAD
    test()();
    test{}();
=======
>>>>>>> 41b0cdc026df773ac91fd7a50f6a4205cb0c7772

    return 0;
}
