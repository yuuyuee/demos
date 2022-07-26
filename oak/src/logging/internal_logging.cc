// Copyright 2022 The Oak Authors.

#include "logging/internal_logging.h"

#include <unistd.h>
#include "common/fmt.h"

namespace oak {
namespace logging_internal_logging {
namespace {

oak::LogLevel kOakInternalLogLevel = oak::LogLevel::OAK_LOG_LEVEL_ERROR;
}  // anonymous namespace

void SetInternalLogLevel(oak::LogLevel level) {
  kOakInternalLogLevel = level;
}

void InternalLog(const char* level, const char* fname, int line, const char* fmt, ...) {
  constexpr const size_t kMaxBufferSize = 1024;
  char buffer[kMaxBufferSize];
  size_t len = oak::Format(buffer, kMaxBufferSize, "[%s:%d] %7s ", fname, line, level);
  va_list ap;
  va_start(ap, fmt);
  len += oak::Format(buffer + len, kMaxBufferSize - len, fmt, ap);
  va_end(ap);
  write(STDERR_FILENO, buffer, len);
}
}  // namespace logging_internal_logging
}  // namespace oak
