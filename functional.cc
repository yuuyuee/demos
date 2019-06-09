#include <iostream>
#include <functional>

<<<<<<< HEAD
class base {
public:
    base() = default;
    ~base() = default;

    void f() { std::cout << "base::func()" << std::endl; }
    void fc() const { std::cout << "base::func() const" << std::endl; }
    void f1(int v) { std::cout << "base::func(int v) = " << v << std::endl; }
    void f1c(int v) const { std::cout << "base::func(int v) const = " << v << std::endl; }
};

void print1(int v) {
    std::cout << "print(int v) = " << v << std::endl;
}

void print2(int v1, int v2) {
    std::cout << "print(int v1, int v2) = " << v1 << ", " << v2 << std::endl;
}


int main(void) {
    std::pointer_to_unary_function<int, void> unary_func = std::ptr_fun(print1);
    unary_func(1);
    std::pointer_to_binary_function<int, int, void> binary_func = std::ptr_fun(print2);
    binary_func(1, 2);

    base b;
    std::mem_fun_t<void, base> f = std::mem_fun(&base::f);
    f(&b);
    std::const_mem_fun1_t<void, base, int> f1c = std::mem_fun(&base::f1c);
    f1c(&b, 2);
    std::mem_fun_ref_t<void, base> fc = std::mem_fun_ref(&base::f);
    fc(b);

    std::reference_wrapper<base> br = std::ref(b);
    br.get().f1(100);
=======
template<typename Tp, typename FuncTp = std::less<Tp> >
bool min(const Tp &lhl, const Tp &rhl, FuncTp func = FuncTp()) {
    return func(lhl, rhl);
}

int main(void) {
    const int a = 20, b = 10;
    std::cout << min(10, 20) << std::endl;
    std::cout << min(a, b) << std::endl;

>>>>>>> 41b0cdc026df773ac91fd7a50f6a4205cb0c7772
    return 0;
}
