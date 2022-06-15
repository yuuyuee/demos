#include <iostream>

template <typename _Tp, bool v = true>
struct Compare {
    bool operator()(const _Tp&, const _Tp&) {
        std::cout << "Template operator()" << std::endl;
        return true;
    }
};

template <typename _Tp>
struct Compare<_Tp, false> {
    bool operator()(const _Tp&, const _Tp&) {
        std::cout << "Partial template operator()" << std::endl;
        return false;
    }
};

int main(void) {
    Compare<int, false>{}(0, 0);
    Compare<int, true>{}(0, 0);
    Compare<int>{}(0, 0);
    return 0;
}
