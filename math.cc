#include <math.h>
#include <iostream>

int main() {
  for (double x = -1; x <= 1; x += 0.1) {
    std::cout << "(" << x << ", " << sin(x) * 10 << ")" << std::endl;
  }
  return 0;
}
