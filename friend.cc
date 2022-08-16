#include <iostream>

class Object {
 public:
  template <typename Tp>
  friend void Print(const Object&);

  Object(): i_(100) {}

 private:
  int i_;
};

template <typename Tp>
void Print(const Object& o) {
  std::cout << o.i_ << std::endl;
}

int main() {
  Object o;
  Print<int>(o);
  Print<Object>(o);
  return 0;
}
