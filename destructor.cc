#include <iostream>
#include <new>

class glass {
public:
    glass() {
        std::cout << "glass constructor" << std::endl;
    }
    ~glass() {
        std::cout << "glass destructor" << std::endl;
    }
};

int main(void) {
    glass *g = NULL;

    g = static_cast<glass *>(::operator new(sizeof(glass)));
    if (g) {
        ::new(static_cast<void *>(g)) glass;
        g->~glass();
        ::operator delete(g);
    }

    return 0;
}
