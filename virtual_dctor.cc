#include <iostream>

class Base {
 public:
  Base() = default;
  virtual ~Base() = default;

  virtual void Func() const = 0;
};

class Derived: public Base {
 public:
  Derived() { std::cout << "Derived::Derived()" << std::endl; }
  virtual ~Derived() { std::cout << "Derived::~Derived()" << std::endl; }

  virtual void Func() const { std::cout << "Derived::Func()" << std::endl; }
};

int main() {
  Base* ptr = new Derived;
  ptr->Func();
  delete ptr;
  return 0;
}
