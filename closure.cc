#include <memory>
#include <utility>
#include <iostream>
#include <functional>

class Object {
 public:
  Object() { std::cout << "Object::Object: " << this << std::endl; }
  ~Object() { std::cout << "Object::~Object: " << this << std::endl; }

  int Add(int a, int b) { return a + b; }
};


std::function<int(int, int)> GetAddFn() {
  std::cout << "entry GetAddFn()" << std::endl;
  std::shared_ptr<Object> obj = std::make_shared<Object>();
  std::cout << "leave GetAddFn()" << std::endl;
  return std::bind(&Object::Add, obj, std::placeholders::_1, std::placeholders::_2);
}

std::function<int(int, int)> GetAddFn2() {
  std::cout << "entry GetAddFn2()" << std::endl;
  std::shared_ptr<Object> obj = std::make_shared<Object>();
  std::cout << "leave GetAddFn2()" << std::endl;
  return [obj](int a, int b) ->int { return obj->Add(a, b); };
}

int main() {
  std::cout << "entry main()" << std::endl;
  int a = 100, b = 50;
  auto fn = GetAddFn();
  std::cout << "fn(" << a << ", " << b << ") = " << fn(a, b) << std::endl;
  auto fn2 = GetAddFn2();
  std::cout << "fn2(" << a << ", " << b << ") = " << fn2(a, b) << std::endl;
  std::cout << "leave main()" << std::endl;

  return 0;
}
