#include <iostream>
#include <typeinfo>

<<<<<<< HEAD
int main(void) {
    int i = 0;
    int& r = i;
    int&& rr = 0;
    std::cout << typeid(i).name() << std::endl;
    std::cout << typeid(r).name() << std::endl;
    std::cout << typeid(rr).name() << std::endl;
=======
template<typename Tp>
struct test {
    Tp v;
    template<typename Tp1>
        bool cmp(const test<Tp1>& t) {
            std::cout << typeid(v).name() << std::endl;
            return typeid(v) == typeid(t.v);
        }
};

int main(void) {
    test<int> t1;
    test<int> t2;
    test<char> t3;

    std::cout << (t1.cmp(t2)) << std::endl;
    std::cout << (t1.cmp(t3)) << std::endl;

>>>>>>> 41b0cdc026df773ac91fd7a50f6a4205cb0c7772
    return 0;
}

