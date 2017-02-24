#include <iostream>
#include <initializer_list>

class test {
public:
    ~test() {}

    test() {
        std::cout << "Default Construct Function" << std::endl;
    }

    explicit test(const test &t) {
        std::cout << "Copy Construct Function" << std::endl;
    }

    explicit test(const std::initializer_list<int> &) {
        std::cout << "List Construct Function" << std::endl;
    }

    test &operator=(const test &t) {
        std::cout << "Assignment Function" << std::endl;
        return *this;
    }
};

int main(void) {
    test a;
    test b(a);
//    test c = a;
    test d;
    d = a;
    test e{1, 2, 3};

    return 0;
}
