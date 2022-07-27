// Copyright 2022 The Oak Authors.

#include "common/panic.h"

#include <unistd.h>
#include <string.h>

#include "common/fmt.h"

namespace oak {
namespace panic_internal {
namespace {

constexpr const char kTailMsg[] = "... (message truncated)\n";
constexpr const size_t kTailMsgSize = sizeof(kTailMsg);
}  // anonymous namespace

void PanicLog(const char* fname, int line, const char* fmt, ...) {
  constexpr const size_t kBufferSize = 2048;
  char buffer[kBufferSize];

  size_t plen = Format(buffer, kBufferSize, "P [%s:%d] ", fname, line);

  va_list ap;
  va_start(ap, fmt);
  size_t len = Format(buffer + plen, kBufferSize - plen, fmt, ap);
  if (len > kBufferSize - plen) {
    memcpy(buffer + kBufferSize - kTailMsgSize, kTailMsg, kTailMsgSize);
    // snprintf and vsnprintf wite at most size bytes inlcude
    // the terminating null byte '\0'.
    len = kBufferSize - plen - 1;
  }
  va_end(ap);

  write(STDERR_FILENO, buffer, plen + len);
}

}  // namespace panic_internal
}  // namespace oak
