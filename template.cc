#include <iostream>
#include <utility>

struct Derive {
    Derive() { std::cout << "derive constructor" << std::endl; }
    ~Derive() { std::cout << "derive destructor" << std::endl; }

    int func(int i) const { std::cout << "derive args: " << i << std::endl; return i + 1; }
private:
    Derive(const Derive&);
    void operator=(const Derive&);
};

void f(const Derive& d) {
    d.func(100);
}

template<typename Tp, size_t N>
void DumpArray(const Tp* v) {
    std::cout << "Array: ";
    for (size_t i = 0; i < N; ++i) {
        std::cout << v[i] << " ";
    }
    std::cout << std::endl;
}

int main(void) {
    Derive p;
    f(p);
    int iv[10];
    DumpArray<int, 10>(iv);
    return 0;
}
