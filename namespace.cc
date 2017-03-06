#include <iostream>

namespace test {
namespace {
    enum {
        BLUE = 0,
        RED  = 1,
    };

    class test {
    public:
        test();
        ~test();
    };
}
}

test::test::test() {
    std::cout << "test constrctor" << std::endl;
}
test::test::~test() {
    std::cout << "test destrctor" << std::endl;
}

using namespace test;

int main(void) {
    int v = ::RED;
    test::test t;
    std::cout << v << std::endl;

    return 0;
}
