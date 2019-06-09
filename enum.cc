#include <iostream>

struct test {
public:
    enum enum_type {
        none = 0,
        fail_if_exists = none,
        overwrite_if_exists
    };
};

int main(void) {
    test::enum_type t = test::fail_if_exists;
    std::cout << t << std::endl;

    return 0;
}
