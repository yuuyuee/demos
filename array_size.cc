#include <iostream>

//template<typename T, size_t N>
//auto ArraySizeHelper(const T (&array)[N]) -> char (&)[N];
template<typename T, size_t N, typename Res = char (&)[N]>
Res ArraySizeHelper(const T (&)[N]);

#define ARRAY_SIZE(array) sizeof(ArraySizeHelper(array))

int main() {
  int iv[10];
  double dv[5];

  std::cout << ARRAY_SIZE(iv) << std::endl;
  std::cout << ARRAY_SIZE(dv) << std::endl;
  return 0;
}
