#include <iostream>
// syntax:
// |capture list| |parameter list| |  return type | |function body|
// ----------------------------------------------------------------
// [capture list] (parameter list) -> return type   {function body}
// capture list: optional
//      auto type variable: need captured
//      static type variable: no
//
//      capture type: NAME(value capture)
//                    &NAME(reference capture)
//                    =(implicit value capture)
//                    &(implicit reference capture)
// parameter list: optional
// return type: optional
// function body: must

#include <iostream>

class test {
public:
    test() = default;
    ~test() = default;

    void ops() { std::cout << "test::ops()" << std::endl; }
    void run_ops() {
        auto f = [this](){this->ops();};
        f();
    }
};

int main(void) {
    int a = 20, b = 10;
    auto Min = [](int a, int b) {return a < b ? a : b;};
    auto Max = [](int a, int b) {return a > b ? a : b;};
    auto Add = [a, b]() -> int {return a + b;};
    auto Sub = [&a, &b]() -> int {return a - b;};
    auto Mul = [=]() -> int {return a * b;};
    auto Div = [&]() -> int {return a / b;};

    std::cout << "Min(20, 10): " << Min(a, b) << std::endl;
    std::cout << "Max(20, 10): " << Max(a, b) << std::endl;
    std::cout << "Add(20, 10): " << Add() << std::endl;
    std::cout << "Sub(20, 10): " << Sub() << std::endl;
    std::cout << "Mul(20, 10): " << Mul() << std::endl;
    std::cout << "Div(20, 10): " << Div() << std::endl;
    test().run_ops();
    return 0;
}
