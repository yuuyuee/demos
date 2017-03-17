#include <iostream>
#include <utility>

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

class test {
public:
    test() {
        std::cout << "constructor" << std::endl;
    }
    ~test() {
        std::cout << "constructor" << std::endl;
    }
    inline void print() const {
        std::cout << "print function" << std::endl;
    }
};

template<typename T>
class Value {
public:
    Value(T v): v_(v) {}
    T GetV() const {return v_;}

private:
    T v_;
};

template<typename T>
using Value = Value<T>;

void dump(const test &t) {
    t.print();
}

int main(void) {
    derived d;

    d.v0 = 100;
    d.func0();

    d.func();

#if defined(nullptr_t)
        std::cout << "defined nullptr "<< std::endl;
#endif

    int value = 100;
    std::cout << "value: " << reinterpret_cast<int &&>(value) << std::endl;

#ifdef _LP64
    std::cout << "platform 64" << std::endl;
#endif

    std::cout << "__cplusplus = " << __cplusplus << std::endl;
#ifdef __linux__
    std::cout << "defined __linux__" << std::endl;
#endif
#ifdef __unix__
    std::cout << "defined __unix__" << std::endl;
#endif
#ifdef __GNUG__
    std::cout << "defined __GNUG__: " << __GNUG__ << std::endl;
#endif
#ifdef __GNUC__
    std::cout << "defined __GNUC__: " << __GNUC__ << std::endl;
#endif
#ifdef __GNUC_MINOR__
    std::cout << "defined __GNUC_MINOR__: " << __GNUC_MINOR__ << std::endl;
#endif
#ifdef __STDC__
    std::cout << "defined __STDC__: " << __STDC__ << std::endl;
#endif
#ifdef __VERSION__
    std::cout << "defined __VERSION__: " << __VERSION__ << std::endl;
#endif
#ifdef __USE_GNU
    std::cout << "defined __USE_GNU: " << __USE_GNU << std::endl;
#endif


    dump(test());
    std::cout << "dump end" << std::endl;

    std::string a("hello world");
    std::string b = std::move(a);

    std::cout << a << " " << b << std::endl;

    std::cout << Value<int>(100).GetV() << std::endl;
    return 0;
}
