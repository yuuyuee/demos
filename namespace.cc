#include <iostream>

namespace ns {
    class Test {
    public:
        Test();
        ~Test();
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

ns::Test::Test() {}
ns::Test::~Test() {}

using ns::Test;
using ns::print;

int main(void) {
    int v = ns::RED;
    Test::Test t;
    std::cout << v << std::endl;
    print();

    return 0;
}
