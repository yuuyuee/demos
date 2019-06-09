#include <iostream>
#include <functional>
#include <utility>

class Closure {
public:
    template<typename Callable, typename... Args>
    Closure(Callable&& c, Args&&... args):
        function_(std::bind(std::forward<Callable>(c), std::forward<Args>(args)...)) {}

    void operator()() const {
        function_();
    }

private:
    std::function<void(void)> function_;
};

void no_args() {
    std::cout << "No arguments" << std::endl;
}

void have_args(int args) {
    std::cout << "Have arguments: " << args << std::endl;
}

int main(void) {
    Closure cb0(no_args);
    cb0();
    Closure cb1(have_args, 10);
    cb1();
}
