#include <iostream>
#include <typeinfo>

template<typename Tp>
struct test {
    Tp v;
    template<typename Tp1>
        bool cmp(const test<Tp1>& t) {
            return typeid(v) == typeid(t.v);
        }
};

int main(void) {
    test<int> t1;
    test<int> t2;
    test<char> t3;

    std::cout << (t1.cmp(t2)) << std::endl;
    std::cout << (t1.cmp(t3)) << std::endl;

    return 0;
}

