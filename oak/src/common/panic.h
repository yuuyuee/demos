// Copyright 2022 The Oak Authors.

#ifndef COMMON_PANIC_H_
#define COMMON_PANIC_H_

#include <stdlib.h>
#include "common/macros.h"

// OAK_PANIC
//
// This is are useful to call at any point during initialization,
// they do not allocate memory or acquire locks, and are async-signal
// safe and very few other dependencies.

#define OAK_PANIC(...) do {                               \
  oak::panic_internal::PanicLog(                          \
      oak::panic_internal::Basename(                      \
          __FILE__, sizeof(__FILE__) - 1),                \
      __LINE__, __VA_ARGS__);                             \
  abort();                                                \
} while (0)

#define OAK_CHECK(cond, ...) do {                         \
  if (OAK_EXPECT_FALSE(!(cond)))                          \
    OAK_PANIC("Check " # cond " failed: " __VA_ARGS__);   \
} while (0)

namespace oak {
namespace panic_internal {

// Strip directory which any leading directory compoents has removed.
constexpr const char* Basename(const char* fname, int off) {
  return off == 0 || fname[off - 1] == '/'
      ? fname + off : Basename(fname, off - 1);
}

// OAK_RAW_LOG implement which does not allocate memory or acquire locks.
void PanicLog(const char* fname, int line, const char* fmt, ...)
    OAK_ATTR_PRINTF(3, 4);
}  // namespace panic_internal
}  // namespace oak

#endif  // COMMON_PANIC_H_
