
#ifndef OAK_INTERNAL_MACROS_H_
#define OAK_INTERNAL_MACROS_H_

#define OAK_ARRAY_SIZE(a) \
  (sizeof(a) / sizeof(*(a)) * !(sizeof(a) % sizeof(*(a))))

#include "oak/internal/compiler.h"

#define OAK_ATTR_ALWAYS_INLINE    inline __attribute__((always_inline))
#define OAK_ATTR_NORETURN         __attribute__((noreturn))
#define OAK_ATTR_DEPRECATED(...)  __attribute__((deprecated(__VA_ARGS__)))
#define OAK_ATTR_MAYBE_UNUSED     __attribute__((unused))
#define OAK_ATTR_NODISCARD        __attribute__((warn_unused_result))
#define OAK_ATTR_ALIGNED(size)    __attribute__((aligned(size)))
#define OAK_ATTR_PACKED           __attribute__((packed))
#define OAK_EXPECT_TRUE(x)        __builtin_expect(!!(x), 1)
#define OAK_EXPECT_FALSE(x)       __builtin_expect(!!(x), 0)

#include "oak/internal/platform.h"

#define OAK_CACHELINE_SIZE 64
#define OAK_ATTR_CACHELINE_ALIGNED OAK_ATTR_ALIGNED(OAK_CACHELINE_SIZE)

#endif /* OAK_INTERNAL_MACROS_H_ */
