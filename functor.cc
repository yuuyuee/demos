#include <iostream>
<<<<<<< HEAD
#include <functional>

int sum(int a1, int a2) {
    return a1 + a2;
}

int main(void) {
    std::function<int(void)> f = std::bind(sum, 1, 2);
    std::cout << f() << std::endl;
=======

template <typename T>
class print {
public:
    operator T() const {
        std::cout << "Type call" << std::endl;
        return reinterpret_cast<T>(0);
    }
    T operator()() const {
        std::cout << "Value call" << std::endl;
        return reinterpret_cast<T>(0);
    }
};

int main(void) {
    print<int> pi;

    print<int>();
    pi();

>>>>>>> 41b0cdc026df773ac91fd7a50f6a4205cb0c7772
    return 0;
}
