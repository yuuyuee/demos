#include <iostream>
#include <memory>
#include <new>

#define CHECK_EMPTY(p) do {                             \
    if (!(p)) {                                         \
        std::cout << #p " is null ptr" << std::endl;    \
    } else {                                            \
        std::cout << #p " is not null ptr" << std::endl;\
    }                                                   \
} while (0)

int main(void) {
    std::shared_ptr<int> ip;

    CHECK_EMPTY(ip);
    ip = std::make_shared<int>(100);
    CHECK_EMPTY(ip);
    ip.reset();
    CHECK_EMPTY(ip);
    ip.reset(new int);
    CHECK_EMPTY(ip);
    ip.reset();
    CHECK_EMPTY(ip);

    return 0;
}
