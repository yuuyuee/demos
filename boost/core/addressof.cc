#include <iostream>
#include <memory>
#include <boost/core/addressof.hpp>

namespace {
void func0() {}
void func1(int) {}
void func2(int, int) {}
void func3(int, int, int) {}
}

#define CHECK_MSG(cond, truemsg, falsemsg)  \
    if (cond)                               \
        std::cout << truemsg << std::endl;  \
    else                                    \
        std::cout << falsemsg << std::endl; \

#define CHECK(cond) CHECK_MSG(cond, #cond " - True", #cond " - False")

int main(void) {
    CHECK(boost::addressof(func0) == &func0);
    CHECK(boost::addressof(func1) == &func1);
    CHECK(boost::addressof(func2) == &func2);
    CHECK(boost::addressof(func3) == &func3);

    return 0;
}
