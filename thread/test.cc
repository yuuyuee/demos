#include <iostream>

int main(void) {
#if defined(__GXX_EXPERIMENTAL_CXX0X__)
  std::cout << __GXX_EXPERIMENTAL_CXX0X__ << std::endl;
#endif
#if defined(__cplusplus)
  std::cout << __cplusplus << std::endl;
#endif
  return 0;
}
