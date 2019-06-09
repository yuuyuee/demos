#include <iostream>

template<typename Char>
constexpr size_t strlen(const Char* s, size_t len) {
  return *s == '\0' ? len : strlen(s + 1, len + 1);
}

int main() {
  size_t size = strlen("123456", 0);
  std::cout << size << std::endl;
  return 0;
}
