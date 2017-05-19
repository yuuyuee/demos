#include <iostream>
#include <initializer_list>

class test {
public:
    ~test() { std::cout << "Default destruct Function" << std::endl; }
    test(): m_i(0) { std::cout << "Default construct Function" << std::endl; }
private:
    int m_i;
};

int main(void) {
    test a;
    test b;
    b = a;
    return 0;
}
