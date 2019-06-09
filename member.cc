#include <iostream>

class A {
public:
    A() {}
    virtual ~A() {}
    void f0() const {
        std::cout << "Class A::f0" << std::endl;
    }

    virtual void f1() const {
        std::cout << "Class A::f1" << std::endl;
    }

    virtual void f2() const = 0;
};

class B: public A {
public:
    B(): A() {}
    ~B() {}
    void f0() const {
        std::cout << "Class B::f0" << std::endl;
    }

    virtual void f1() const {
        std::cout << "Class B::f1" << std::endl;
    }

    virtual void f2() const {
        std::cout << "Class B::f2" << std::endl;
    }
};

int main(void) {
    B b;
    A* a = &b;
    a->f0();
    a->f1();
    a->f2();
    return 0;
}
