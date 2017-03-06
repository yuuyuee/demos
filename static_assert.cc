#include <iostream>

template<bool>
struct CompileAssert {};

template<>
struct CompileAssert<true> {};

#define STATIC_ASSERT(expr, msg) \
    ::CompileAssert<bool(expr)> {};


int main(void) {
    //const int i = 0;
    //static_assert(i, "zero");
    CompileAssert t1;

    (void)t1;

    return 0;
}
