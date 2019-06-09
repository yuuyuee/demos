#include <iostream>
#include <functional>

int sum(int a1, int a2) {
    return a1 + a2;
}

int main(void) {
    std::function<int(void)> f = std::bind(sum, 1, 2);
    std::cout << f() << std::endl;
    return 0;
}
