// Author: yuyue/X3130 (yuyue2200@hotmail.com)
// File: weak_ptr.cc

#include <iostream>
#include <memory>
#include <string>
#include <cassert>

class test {
public:
  test() { std::cout << "test constructor" << std::endl; }
  ~test() { std::cout << "test destructor" << std::endl; }
};

template<typename Tp>
void PrintWeakPtr(const std::weak_ptr<Tp>& wptr) {
  auto sptr = wptr.lock();
  if (sptr) {
    std::cout << *sptr << ", " << sptr.use_count() << std::endl;
  } else {
    std::cout << "shared pointer empty" << std::endl;
  }
}

class Base: public std::enable_shared_from_this<Base> {
public:
  Base() { std::cout << "Base constructor" << std::endl; }
  ~Base() { std::cout << "Base destructor" << std::endl; }

  void Print() const {
    std::cout << "Base::Print()" << std::endl;
  }

  std::shared_ptr<Base> Gettor() {
    return this->shared_from_this();
  }
};

int main() {
  std::shared_ptr<int> ptr(new int);
  *ptr = 100;

  std::weak_ptr<int> wptr(ptr);

  PrintWeakPtr(wptr);
  ptr.reset();
  PrintWeakPtr(wptr);

  std::unique_ptr<int> uptr;
  (void)uptr;

  std::shared_ptr<void> vptr;
  {
    std::shared_ptr<test> tptr(new test);
    vptr = tptr;
    std::cout << "come out scope" << std::endl;
  }
  (void)vptr;

  void* vp;
  int* ip = nullptr;
  vp = ip;
  (void)vp;
  (void)ip;

  std::shared_ptr<Base> bptr(new Base);
  bptr->Print();
  {
    auto sptr = bptr->Gettor();
    sptr->Print();
  }
  return 0;
}


