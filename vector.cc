#undef _GLIBCXX_CONCEPT_CHECKS
#include <iostream>
#include <vector>

class test {
public:
  test() {
    std::cout << "Constructor '" << this << "'" << std::endl;
  }
  ~test() {
    std::cout << "Destructor '" << this << "'" << std::endl;
  }
  test(test const& o) {
    std::cout << "Copy constructor from '" << &o << "' to '" << this << "'" << std::endl;
  }
  test& operator =(test const& o) {
    std::cout << "Assign constructor from '" << &o << "' to '" << this << "'" << std::endl;
    return *this;
  }
};

int main(void) {
  std::vector<test> vec;
  test v;
  std::cout << "---------------" << std::endl;
  vec.push_back(v);
  vec.push_back(v);
  vec.push_back(v);
  std::cout << "---------------" << std::endl;
  vec.clear();
  return 0;
}

