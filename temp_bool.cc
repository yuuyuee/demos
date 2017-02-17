#include <iostream>

template <typename _Tp, bool v= true>
struct _cmp {
    bool cmp(const _Tp& a, const _Tp& b) {
        std::cout << "Not have operator" << v << std::endl;
        return true;
    }
};

template <typename _Tp>
struct _cmp<_Tp, false> {
    bool cmp(const _Tp& a, const _Tp& b) {
        std::cout << "Have operator" << std::endl;
        return false;
    }
};

int main(void) {
    _cmp<int, false> a;
    _cmp<int, true> b;
    _cmp<int> c;

    std::cout << a.cmp(0, 0) << std::endl;
    std::cout << b.cmp(0, 0) << std::endl;
    std::cout << c.cmp(0, 0) << std::endl;
    return 0;
}
