#include <iostream>
#include <algorithm>

struct test {
  int a;
  char b;
};

void Print(const test* v, size_t size) {
  for (size_t i = 0; i < size; ++i)
    std::cout << "'" << v[i].a << ", " << v[i].b << "' ";
  std::cout << std::endl;
}

int main() {
  test c[4] = {
    {100, 97},
    {100, 97},
    {100, 97},
    {100, 97}
  };
  Print(c, 4);
  std::fill_n(reinterpret_cast<char*>(c), sizeof(test) * 2, 0);
  Print(c, 4);

  return 0;
}
