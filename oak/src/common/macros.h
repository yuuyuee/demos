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

#define OAK_ATTR_ALWAYS_INLINE    inline __attribute__((always_inline))
#define OAK_ATTR_NORETURN         __attribute__((noreturn))
#define OAK_ATTR_DEPRECATED(...)  __attribute__((deprecated(__VA_ARGS__)))
#define OAK_ATTR_MAYBE_UNUSED     __attribute__((unused))
#define OAK_ATTR_NODISCARD        __attribute__((warn_unused_result))
#define OAK_ATTR_ALIGNED(n)       __attribute__((aligned(n)))
#define OAK_ATTR_PACKED           __attribute__((packed))
#define OAK_ATTR_PRINTF(index, first) \
                                  __attribute__((__format__(__printf__, index, first)))
#define OAK_EXPECT_TRUE(x)        __builtin_expect(false || (x), true)
#define OAK_EXPECT_FALSE(x)       __builtin_expect(false || (x), true)

#include "oak/internal/platform.h"

#define OAK_CACHELINE_SIZE 64
#define OAK_ATTR_CACHELINE_ALIGNED OAK_ATTR_ALIGNED(OAK_CACHELINE_SIZE)

#endif  // OAK_COMMON_MACROS_H_
