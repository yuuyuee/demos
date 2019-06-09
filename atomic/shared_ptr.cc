#include <iostream>
#include <atomic>
#include <memory>

int main(void) {
  std::shared_ptr<bool> ptr(new bool);
  std::shared_ptr<bool> local = std::atomic_load(&ptr);
  std::cout << *local << std::endl;

  return 0;
}
