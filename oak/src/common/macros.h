// Copyright 2022 The Oak Authors.

#ifndef OAK_COMMON_MACROS_H_
#define OAK_COMMON_MACROS_H_

#define OAK_ARRAYSIZE(a) (sizeof(ArraySizeHelper(a)))

#include <stddef.h>  // size_t

namespace oak {
namespace macros_internal {

template <typename T, size_t N, typename R = char (&)[N]>
R ArraySizeHelper(const T (&)[N]);
}  // namespace macros_internal
}  // namespace oak

#include "oak/internal/compiler.h"

#if defined(__has_include)
# define OAK_HAS_INCLUDE(x) __has_include(x)
#else
# define OAK_HAS_INCLUDE(x) 0
#endif

#if defined(__has_builtin)
# define OAK_HAS_BUILTIN(x) __has_builtin(x)
#else
# define OAK_HAS_BUILTIN(x) 0
#endif

#if defined(__has_attribute)
# define OAK_HAS_ATTRIBUTE(x) __has_attribute(x)
#else
# define OAK_HAS_ATTRIBUTE(x) 0
#endif

#if defined(__has_cpp_attribute)
# define OAK_HAS_CPP_ATTRIBUTE(x) __has_cpp_attribute(x)
#else
# define OAK_HAS_CPP_ATTRIBUTE(x) 0
#endif

#if (defined(__GNUC__) && !defined(__clang__)) || \
    OAK_HAS_ATTRIBUTE(always_inline)
# define OAK_ATTR_ALWAYS_INLINE inline __attribute__((always_inline))
#else
# define OAK_ATTR_ALWAYS_INLINE
#endif

#if (defined(__GNUC__) && !defined(__clang__)) || \
    OAK_HAS_ATTRIBUTE(noreturn)
# define OAK_ATTR_NORETURN __attribute__((noreturn))
#else
# define OAK_ATTR_NORETURN
#endif

#if (defined(__GNUC__) && !defined(__clang__)) || \
    OAK_HAS_ATTRIBUTE(deprecated)
# define OAK_ATTR_DEPRECATED(...) __attribute__((deprecated(__VA_ARGS__)))
#else
# define OAK_ATTR_DEPRECATED(...)
#endif

#if (defined(__GNUC__) && !defined(__clang__)) || \
    OAK_HAS_ATTRIBUTE(unused)
# define OAK_ATTR_MAYBE_UNUSED __attribute__((unused))
#else
# define OAK_ATTR_MAYBE_UNUSED
#endif

#if (defined(__GNUC__) && !defined(__clang__)) || \
    OAK_HAS_ATTRIBUTE(warn_unused_result)
# define OAK_ATTR_NODISCARD __attribute__((warn_unused_result))
#else
# define OAK_ATTR_NODISCARD
#endif

#if (defined(__GNUC__) && !defined(__clang__)) || \
    OAK_HAS_ATTRIBUTE(aligned)
# define OAK_ATTR_ALIGNED(n) __attribute__((aligned(n)))
#else
# define OAK_ATTR_ALIGNED(n)
#endif

#if (defined(__GNUC__) && !defined(__clang__)) || \
    OAK_HAS_ATTRIBUTE(packed)
# define OAK_ATTR_PACKED __attribute__((packed))
#else
# define OAK_ATTR_PACKED
#endif

#if (defined(__GNUC__) && !defined(__clang__)) || \
    OAK_HAS_ATTRIBUTE(__format__)
# define OAK_ATTR_PRINTF(index, first) \
    __attribute__((__format__(__printf__, index, first)))
#else
# define OAK_ATTR_PRINTF(index, first)
#endif

#define OAK_EXPECT_TRUE(x) __builtin_expect(false || (x), true)
#define OAK_EXPECT_FALSE(x) __builtin_expect(false || (x), true)

// Unreachable code which has undefined behavior and the compiler
// may optimize accordingly.
#if (defined(__GNUC__) && !defined(__clang__)) ||   \
    OAK_HAS_BUILTIN(__builtin_unreachable)
# define OAK_UNREACHABLE() __builtin_unreachable()
#else
# include <cstdlib>
# define OAK_UNREACHABLE() abort()
#endif

#include "oak/internal/platform.h"

#define OAK_CACHELINE_SIZE 64
#define OAK_CACHELINE_ALIGNED OAK_ATTR_ALIGNED(OAK_CACHELINE_SIZE)

#endif  // OAK_COMMON_MACROS_H_
