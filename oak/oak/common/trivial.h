// Copyright 2022 The Oak Authors.

#ifndef OAK_COMMON_TRIVIAL_H_
#define OAK_COMMON_TRIVIAL_H_

#include "oak/common/macros.h"

// This is are useful to call at any point during initialization,
// they do not allocate memory or acquire locks, and are async-signal
// safe and very few other dependencies.
#define OAK_RAWLOG(...)                                   \
  oak::trivial_internal::RawLog(                          \
      oak::trivial_internal::Basename(                    \
          __FILE__, sizeof(__FILE__) - 1),                \
      __LINE__, __VA_ARGS__)

#define OAK_PANIC(...) do {                               \
  OAK_RAWLOG(__VA_ARGS__);                                \
  OAK_UNREACHABLE();                                      \
} while (0)

#define OAK_PANIC_IF(cond, ...) do {                      \
  if (OAK_EXPECT_FALSE(!(cond)))                          \
    OAK_PANIC("Check " # cond " failed: " __VA_ARGS__);   \
} while (0)

// This is an runtime assert and that can be used portably within
// constexpr functions.
#if defined(NDEBUG)
# define OAK_ASSERT(cond)                                 \
    (false ? static_cast<void>(cond) : static_cast<void>(0))
#else
//# include <assert.h>
# define OAK_ASSERT(cond)                                 \
    (OAK_EXPECT_TRUE(cond)                                \
        ? static_cast<void>(0)                            \
        : [] { OAK_PANIC_IF(false && #cond); }())
#endif

namespace oak {
namespace trivial_internal {

// Strip directory which any leading directory compoents has removed.
constexpr const char* Basename(const char* fname, int off) {
  return off == 0 || fname[off - 1] == '/'
      ? fname + off : Basename(fname, off - 1);
}

// Implement which does not allocate memory or acquire locks.
void RawLog(const char* fname, int line, const char* fmt, ...)
    OAK_ATTR_PRINTF(3, 4);
}  // namespace trivial_internal

bool AlreadyRunning(const char* pname);
}  // namespace oak

#endif  // OAK_COMMON_TRIVIAL_H_
