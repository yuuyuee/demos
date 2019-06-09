#include <iostream>

class test {
public:
    test() { std::cout << "test::constructor" << std::endl; }
    ~test() { std::cout << "test::destructor" << std::endl; }
};

template<typename Class>
const Class& empty_class() {
    static Class obj;
    return obj;
}

int main() {
    const test& t0 = empty_class<test>();
    const test& t1 = empty_class<test>();
    const test& t2 = empty_class<test>();
    (void)t0;
    (void)t1;
    (void)t2;
    return 0;
}
