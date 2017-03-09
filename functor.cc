#include <iostream>

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

    return 0;
}
