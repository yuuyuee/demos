#include <iostream>

class base {
public:
    base() =default;
    ~base() =default;

    int v0;
    void func0() {std::cout << "Call func0" << std::endl;}
protected:
    int v1;
    void func1() {std::cout << "Call func1" << std::endl;}
private:
    int v2;
    void func2() {std::cout << "Call func2" << std::endl;}
};

class derived: public base {
public:
    derived() = default;
    ~derived() = default;

    void func() {
        base::v1 = 100;
        base::func1();
    }
};

int main(void) {
    derived d;

    d.v0 = 100;
    d.func0();

    d.func();

#if defined(nullptr_t)
        std::cout << "defined nullptr "<< std::endl;
#endif

    return 0;
}
