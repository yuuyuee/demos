#include <iostream>

class Other {
 public:
  Other() = default;
  virtual ~Other() = default;

  virtual void f0() noexcept = 0;
};

class O2: public Other {
 public:
  O2() = default;
  virtual ~O2() = default;

  virtual void f0() noexcept {
    std::cout << "O2::f0" << std::endl;
  }
};

class Base: public Other {
 public:
  Base() = default;
  virtual ~Base() = default;

  virtual void f1() noexcept = 0;
};


class Derived: public Base {
 public:
  Derived() = default;
  virtual ~Derived() = default;

  virtual void f0() noexcept {
    std::cout << "Derived::f0" << std::endl;
  }

  virtual void f1() noexcept {
    std::cout << "Derived::f1" << std::endl;
  }
};

int main() {
  Base* p = new Derived;
  p->f0();
  p->f1();
  delete p;
  return 0;
}
