#include <iostream>

template <typename Tp>
class Base {
 public:
  Base() = default;
  ~Base() = default;

  void Method() { static_cast<Tp*>(this)->MethodImpl(); }
};

class Derived: public Base<Derived> {
 public:
  Derived() = default;
  ~Derived() = default;

  void MethodImpl() { std::cout << "Derived::MethodImpl()" << std::endl; }
};

template <typename D>
void Method(Base<D>& o) { o.Method(); }

int main() {
  Derived o;
  Method(o);
  return 0;
}
