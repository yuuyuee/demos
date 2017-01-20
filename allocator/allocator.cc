#include <iostream>
#include <memory>

class test {
public:
    test(int n): v(n) {
        std::cout << "constructor" << std::endl;
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
#   define nullptr NULL
#endif

int main(void) {
    std::allocator<class test> alloc;
    class test *tp = nullptr;

    tp = alloc.allocate(sizeof(*tp), 0);
    alloc.construct(tp, 100);
    tp->print();
    alloc.destroy(tp);
    alloc.deallocate(tp, sizeof(*tp) + sizeof(int));
    return 0;
}
