#include <iostream>
#include <typeinfo>

using namespace std;

template<typename Tp, bool = 0>
class Type {
public:
    void descriptor() {
        cout << "<Tp, bool> is empty ? " << (typeid(Tp) == typeid(void) ? "True" : "false") << endl;
    }
};

template<typename Tp>
class Type<Tp, true> {
public:
    void descriptor() {
        cout << "<Tp> is empty ? True"<< endl;
    }
};

int main(void) {
    Type<void> t0;
    Type<int, false> t1;
    Type<void, true> t2;
    Type<int, true> t3;

    t0.descriptor();
    t1.descriptor();
    t2.descriptor();
    t3.descriptor();

    return 0;
}
