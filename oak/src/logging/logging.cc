// Copyright 2022 The Oak Authors.

#include "logging/logging.h"

#include <unistd.h>
#include <string.h>

#include "common/fmt.h"

namespace oak {
namespace logging_internal {
namespace {

constexpr const char kTailMsg[] = "... (message truncated)\n";
constexpr const size_t kTailMsgSize = sizeof(kTailMsg);
}  // anonymous namespace

void RawLog(const char* fname, int line, char level, const char* fmt, ...) {
  constexpr const size_t kBufferSize = 2048;
  char buffer[kBufferSize];

  size_t len = Format(buffer, kBufferSize, "%c [%s:%d] ", level, fname, line);
  if (OAK_EXPECT_FALSE(len > kBufferSize)) {
    memcpy(buffer + kBufferSize - kTailMsgSize, kTailMsg, kTailMsgSize);
    len = kBufferSize;
  }

  va_list ap;
  va_start(ap, fmt);
  len += Format(buffer + len, kBufferSize - len, fmt, ap);
  va_end(ap);
  write(STDERR_FILENO, buffer, len);
}

}  // namespace logging_internal
}  // namespace oak
