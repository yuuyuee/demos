#include <iostream>
#include <memory>
<<<<<<< HEAD

struct test {
  ~test() {
    std::cout << "test destroy" << std::endl;
  }
};

template<typename T>
struct default_delete {
  void operator()(T* p) {
    delete[] p;
  }
};

int main(void) {
  std::shared_ptr<test> s;
  s.reset(new test[10], std::default_delete<test[]>());
  (void)s;
=======
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

>>>>>>> 41b0cdc026df773ac91fd7a50f6a4205cb0c7772
    return 0;
}
