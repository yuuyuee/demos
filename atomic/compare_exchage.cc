#include <iostream>

int main(void) {
  int old = 1000;
  int ret = __sync_val_compare_and_swap(&old, 1000, 1111);
  std::cout << old << ", " << ret << std::endl;
  ret = __sync_val_compare_and_swap(&old, 2222, 3333);
  std::cout << old << ", " << ret << std::endl;

  return 0;
}
