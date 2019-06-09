#include <iostream>
#include <atomic>
<<<<<<< HEAD
#include <memory>

int main(void) {
  std::atomic<bool> b;
  bool expected = false;
  bool ret = false;

  while (!(ret = b.compare_exchange_weak(expected, true)) && !expected)
    ;
  std::cout << "atomic: " << b << std::endl;
  std::cout << "expected: " << expected << std::endl;
  std::cout << "return: " << ret << std::endl;

  return 0;
=======

using std::cout;
using std::endl;
using std::atomic_int;

int main(void) {
    atomic_int i(0);
    i++;
    cout << "atomic_int is lock-free: " << i.is_lock_free() << endl;
    cout << i << endl;

    return 0;
>>>>>>> 41b0cdc026df773ac91fd7a50f6a4205cb0c7772
}
