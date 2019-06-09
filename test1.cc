#include <iostream>

class test {
public:
    test() {
        std::cout << "Default constructor" << std::endl;
    }
    ~test() = default;
    static const int kConstVariable = 10;
    void Dump() const { std::cout << kConstVariable << std::endl; }
    test(const test&) {
        std::cout << "Copy constructor" << std::endl;
    }

    test& operator =(const test&) {
        std::cout << "Assign constructor" << std::endl;
        return *this;
    }
};

class test1 {
public:
    void f0() {}
    int i;
};

class test2 {
public:
    void f0() {}
    void f1() {}
    int i;
};

class test3 {
public:
    void f0() {}
    void f1() {}
    void f2() {}
    int i;
};

void Dump(const test& t) {
    t.Dump();
}

test MakeTest() {
    return test();
}

int main(void) {
    std::cout << ">> test default constructor" << std::endl;
    test t;
    std::cout << ">> test copy constructor" << std::endl;
    test t1(t);
    std::cout << ">> test assign constructor" << std::endl;
    test t2;
    t2 = t1;
    std::cout << ">> test function return" << std::endl;
    test t3 = MakeTest();
    test t4;
    t4 = MakeTest();

    std::cout << ">> test function parameter" << std::endl;
    Dump(t3);

    t4.Dump();

    test* p = new test[5];
    delete[] p;

    std::cout << sizeof(test1) << " " << sizeof(test2) << " " << sizeof(test3) << std::endl;
    test3 tt3;
    std::cout << &tt3 << std::endl;
    std::cout << &tt3.f0 << std::endl;
    std::cout << tt3.f1 << std::endl;
    std::cout << tt3.f2 << std::endl;
    std::cout << &tt3.i << std::endl;
    return 0;
}
