#include <mutex>

template <typename Tp>
class Singleton {
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
  ptr = new Tp;
}

template <typename Tp>
Tp* Singleton<Tp>::Instance() {
  std::call_once(flag, Init);
  return ptr;
}

//////////////////////////////////////////////////////////////////////
#include <thread>
#include <iostream>
int main() {
  std::thread  t1([]{}); t1.join();
  *(Singleton<int>::Instance()) = 100;
  std::cout <<*(Singleton<int>::Instance()) << std::endl;
  return 0;
}
