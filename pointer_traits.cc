// Author: yuyue/X3130 (yuyue2200@hotmail.com)
// File: pointer_traits.cc

#include <iostream>
#include <memory>

int main() {
  std::shared_ptr<int> sptr(new int{10});
  std::unique_ptr<int> uptr(new int{20});
  std::pointer_traits<int*>::pointer p = nullptr;
  int v = 10;
  std::pointer_traits<std::shared_ptr<int>> sp =
      std::pointer_traits<std::shared_ptr<int>>::pointer_to(v);

  (void)p;
  return 0;
}


