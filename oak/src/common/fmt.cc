// Copyright 2022 The Oak Authors.

#include "common/fmt.h"

#include <stdio.h>
#include <string.h>

namespace oak {
namespace {
constexpr const char kTruncMsg[] = "... (message truncated)\n";
}  // anonymous namespace

size_t Format(char* buffer, size_t len, const char* fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  size_t ret = Format(buffer, len, fmt, ap);
  va_end(ap);
  return ret;
}

size_t Format(char* buffer, size_t len, const char* fmt, va_list ap) {
  int ret = vsnprintf(buffer, len, fmt, ap);
  // assumption always success in here.
  if (ret < 0) {
    ret = 0;
  } else if (ret >= len) {
    memcpy(buffer + len - sizeof(kTruncMsg), kTruncMsg, sizeof(kTruncMsg));
    ret = len;
  }
  return ret;
}

}  // namespace oak
