// Copyright 2022 The Oak Authors.

#include "common/fmt.h"
#include <stdio.h>

namespace oak {
size_t Format(char* buffer, size_t len, const char* fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  size_t ret = VFormat(buffer, len, fmt, ap);
  va_end(ap);
  return ret;
}

size_t VFormat(char* buffer, size_t len, const char* fmt, va_list ap) {
  int ret = vsnprintf(buffer, len, fmt, ap);
  // assumption always success in here.
  return ret >= 0 ? ret : 0;
}

}  // namespace oak
