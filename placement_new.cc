#include <iostream>

class test {
public:
    test(int n) {
        std::cout << "constructor" << std::endl;
        v = n;
    }
    ~test() {
        std::cout << "destructor" << std::endl;
    }

    void print() {
        std::cout << "v = " << v << std::endl;
    }

private:
    int v;
};

#ifndef nullptr
#   include <cstdlib>
#   define nullptr NULL
#endif

int main(void) {
    char *buffer = nullptr;
    class test *tp = nullptr;

    buffer = new char[sizeof(*tp) * 10];

    for (int i = 0; i < 10; i++) {
        tp = new(buffer + i * sizeof(*tp)) test(i);
        tp->print();
        tp->~test();
    }
    delete [] buffer;

    return 0;
}
