#ifndef API
# define API __attribute__ ((visibility ("default")))
#endif

namespace c {
// extern int value_in_c;
void fn_c();
}

namespace x {
void fn() {}
}

namespace a {
API void fn_a();
API void fn_a() {
  // c::value_in_c = 1000;
  c::fn_c();
}
}