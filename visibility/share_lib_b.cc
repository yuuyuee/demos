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

namespace b {
API void fn_b();
void fn_b() {
  // c::value_in_c = 100;
  c::fn_c();
}
}