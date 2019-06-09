#include <iostream>

<<<<<<< HEAD
namespace ns {
    class Test {
    public:
        Test();
        ~Test();
=======
namespace test {
    class test {
    public:
        test();
        ~test();
>>>>>>> 41b0cdc026df773ac91fd7a50f6a4205cb0c7772
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

<<<<<<< HEAD
ns::Test::Test() {}
ns::Test::~Test() {}

using ns::Test;
using ns::print;

int main(void) {
    int v = ns::RED;
    Test::Test t;
=======
test::test::test() {}
test::test::~test() {}

using test::test;
using test::print;

int main(void) {
    int v = test::RED;
    test::test t;
>>>>>>> 41b0cdc026df773ac91fd7a50f6a4205cb0c7772
    std::cout << v << std::endl;
    print();

    return 0;
}
