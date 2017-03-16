#include <iostream>

namespace test {
    class test {
    public:
        test();
        ~test();
    };

namespace {
    enum {
        BLUE = 0,
        RED  = 1,
    };

    void print(void) {
        std::cout << "hello world" << std::endl;
    }
}
}

test::test::test() {}
test::test::~test() {}

using test::test;
using test::print;

int main(void) {
    int v = test::RED;
    test::test t;
    std::cout << v << std::endl;
    print();

    return 0;
}
