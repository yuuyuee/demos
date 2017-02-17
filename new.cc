#include <iostream>
#include <new>
#include <exception>

int main(void) {
    size_t size = (size_t)-1;
    void *p;
    try {
        p = ::operator new(size);
    }catch(std::exception &e) {
        std::cout << e.what() << std::endl;
    }
    ::operator delete(p);
    return 0;
}
