#include <iostream>
#include <initializer_list>

class test {
public:
<<<<<<< HEAD
    test() {
        std::cout << "Default construct Function" << std::endl;
    }
    test(const test& t) {
        std::cout << "Copy construct Function" << std::endl;
    }

    test& operator=(const test& t) {
        std::cout << "Assignment construct Function" << std::endl;
        return *this;
    }
};

int main(void) {
    test a; // default
    test b(a); // copy
    test c; // default
    c = a; // assignment
    test __attribute__((unused)) d = a; // copy
=======
    ~test() { std::cout << "Default destruct Function" << std::endl; }
    test(): m_i(0) { std::cout << "Default construct Function" << std::endl; }
private:
    int m_i;
};

int main(void) {
    test a;
    test b;
    b = a;
>>>>>>> 41b0cdc026df773ac91fd7a50f6a4205cb0c7772
    return 0;
}
