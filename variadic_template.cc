#include <iostream>
#include <cstdio>

template<typename... Args>
void Print(Args&&... args) {
  printf(args...);
}

int main() {
  Print("%d %d %d", 1, 2, 3);
  return 0;
}
