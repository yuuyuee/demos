#include <iostream>
#include <utility>

class Test {
public:
    Test() { std::cout << "Default construct" << std::endl; }
    Test(const Test&) { std::cout << "Copy construct" << std::endl; }
    Test(Test&&) { std::cout << "Move Copy construct" << std::endl; }
    const Test& operator=(const Test&) { std::cout << "Assign construct" << std::endl; return *this; }
    const Test& operator=(Test&&) { std::cout << "Move assign construct" << std::endl; return *this; }
};

void RefFunc(const Test&) {
    std::cout << "At the void RefFunc(const Test&)" << std::endl;
}

Test& RetFunc() {
    std::cout << "At the Test& RetFunc()" << std::endl;
    return *(new Test);
}

void ValueFunc(Test) {
    std::cout << "At the void ValueFunc(Test)" << std::endl;
}

int main() {
    // Default construct
    Test a;

    // Copy construct
    Test b(a);

    Test c;

    // Assign construct
    c = b;

    // Move copy construct
    Test d(std::move(c));
    Test e;
    e = std::move(d); //

    std::cout << "==========================" << std::endl;
    RefFunc(a);
    RefFunc(std::move(a));
    Test __attribute__((unused)) f = RetFunc();
    Test __attribute__((unused)) f1 = std::move(RetFunc());
    //Test& __attribute__((unused)) g = RetFunc();

    // self assignment
    std::cout << "==========================" << std::endl;
    Test __attribute__((unused)) s(s);
    Test __attribute__((unused)) s1 = s1;
    Test __attribute__((unused)) s2(std::move(s2));
    Test __attribute__((unused)) s3 = std::move(s3);

    std::cout << "==========================" << std::endl;
    ValueFunc(e);

    return 0;
}
