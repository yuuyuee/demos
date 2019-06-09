#include <iostream>

template<const char* p>
struct CharNumber {
  static const size_t size = *p == '\0' ? 0 : *p == '*' ? 1 + CharNumber<p + 1>::size : 0 + CharNumber<p + 1>::size;
};

int main() {
  constexpr const char* p = "12*34*";
  std::cout << CharNumber<p>::size << std::endl;
  return 0;
}
