#ifndef API
# define API __attribute__ ((visibility ("default")))
#endif

namespace a {
API void fn_a();
}

namespace b {
API void fn_b();
}

int main() {
  a::fn_a();
  b::fn_b();
  a::fn_a();
  b::fn_b();
  return 0;
}