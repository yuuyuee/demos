#include <iostream>

template<typename Tp>
class test {
public:
    Tp v;
    template<typename Tp1>
        struct rebind {
            Tp1 v;
        };
};

int main(void) {
    typedef typename test<int>::template rebind<char> sub_type;

    sub_type t;
    t.v = 100;
    std::cout << t.v << std::endl;
    return 0;
}
