#include <cassert>
#include <type_traits>
#if defined(USE_PTHREAD)
# include <pthread.h>
#else
# include <mutex>
# include <thread>
#endif

template <typename Tp>
class Singleton {
  static_assert(std::is_constructible<Tp>::value, "");

 public:
  static inline Tp* Instance();

 private:
  static inline void Init();
#if defined(USE_PTHREAD)
  static pthread_once_t flag;
#else
  static std::once_flag flag;
#endif
  static Tp* ptr;
};

template <typename Tp>
#if defined(USE_PTHREAD)
pthread_once_t Singleton<Tp>::flag = PTHREAD_ONCE_INIT;
#else
std::once_flag Singleton<Tp>::flag;
#endif

template <typename Tp>
Tp* Singleton<Tp>::ptr = nullptr;

template <typename Tp>
void Singleton<Tp>::Init() {
  assert(ptr == nullptr);
  ptr = new Tp;
}

template <typename Tp>
Tp* Singleton<Tp>::Instance() {
#if defined(USE_PTHREAD)
  pthread_once(&flag, Init);
#else
  std::call_once(flag, Init);
#endif
  return ptr;
}

//////////////////////////////////////////////////////////////////////
#include <iostream>
int main() {
  *(Singleton<int>::Instance()) = 100;
  std::cout <<*(Singleton<int>::Instance()) << std::endl;
  return 0;
}
