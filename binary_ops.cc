#include <iostream>

class Ops {
public:
    template<typename T>
    Ops(const T& v) {
        std::cout << "Initialize Ops with " << v << std::endl;
    }
    ~Ops() = default;
    Ops(const Ops&) = default;
    Ops& operator =(const Ops&) = default;
};

int main() {
    bool flag = true;
    Ops ops = flag ? Ops(true) : Ops(false);
    (void)ops;
    return 0;
}
