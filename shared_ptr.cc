#include <iostream>
#include <memory>

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
    return 0;
}
