#include <iostream>
#include <functional>

template<typename Tp, typename FuncTp = std::less<Tp> >
bool min(const Tp &lhl, const Tp &rhl, FuncTp func = FuncTp()) {
    return func(lhl, rhl);
}

int main(void) {
    const int a = 20, b = 10;
    std::cout << min(10, 20) << std::endl;
    std::cout << min(a, b) << std::endl;

    return 0;
}
