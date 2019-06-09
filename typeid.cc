#include <iostream>
#include <typeinfo>

int main(void) {
    int i = 0;
    int& r = i;
    int&& rr = 0;
    std::cout << typeid(i).name() << std::endl;
    std::cout << typeid(r).name() << std::endl;
    std::cout << typeid(rr).name() << std::endl;
    return 0;
}

