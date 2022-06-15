
#include <iostream>
#include <cassert>
#include <type_traits>
#include <mutex>  // NOLINT

template <typename Tp>
class Singleton {
  static_assert(std::is_constructible<Tp>::value, "");

 public:
  static inline Tp* Instance();

 private:
  static inline void Init();
  static std::once_flag flag;
  static Tp* ptr;
};

template <typename Tp>
std::once_flag Singleton<Tp>::flag;

template <typename Tp>
Tp* Singleton<Tp>::ptr = nullptr;

template <typename Tp>
void Singleton<Tp>::Init() {
  assert(ptr == nullptr);
  ptr = new Tp;
}

template <typename Tp>
Tp* Singleton<Tp>::Instance() {
  std::call_once(flag, Init);
  return ptr;
}


int main() {
  *(Singleton<int>::Instance()) = 100;
  std::cout <<*(Singleton<int>::Instance()) << std::endl;
  return 0;
}
