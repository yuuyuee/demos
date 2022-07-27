// Copyright 2022 The Oak Authors.

#ifndef OAK_COMMON_FMT_H_
#define OAK_COMMON_FMT_H_

#include <stdarg.h>
#include "common/macros.h"

namespace oak {

size_t Format(char* buffer, size_t len, const char* fmt, ...)
    OAK_ATTR_PRINTF(3, 4);

// vsnprintf called from below function.
size_t VFormat(char* buffer, size_t len, const char* fmt, va_list ap)
    OAK_ATTR_PRINTF(3, 0);

}  // namespace oak

#endif  // OAK_COMMON_FMT_H_
