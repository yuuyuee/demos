// Copyright 2022 The Oak Authors.

#include <unistd.h>  // write
#include <stdarg.h>
#include <atomic>
#include "logging/logging.h"

namespace oak {
namespace logging_internal {
namespace {

constexpr const char kTailMsg[] = "... (message truncated)\n";
constexpr const size_t kTailMsgSize = sizeof(kTailMsg);
std::atomic<Logger> handler(DefaultLogger);

void DefaultLogger(StringPiece msg) {
  write(STDERR_FILENO, msg.data(), msg.size());
}
}  // anonymous namespace

void LogImpl(LogLevel level, const char* fname, int line, const char* fmt, ...) {
  constexpr const int kBufferSize = 2048;
  char buffer[kBufferSize];

  int plen = snprintf(buffer, kBufferSize, "%s [%s:%d] ",
      LogLevelShortName(level), fname, line);

  va_list ap;
  va_start(ap, fmt);
  int len = vsnprintf(buffer + plen, kBufferSize - plen, fmt, ap);
  if (len < 0) len = 0;
  if (len >= kBufferSize - plen) {
    memcpy(buffer + kBufferSize - kTailMsgSize, kTailMsg, kTailMsgSize);
    // snprintf and vsnprintf wite at most size bytes inlcude
    // the terminating null byte '\0'.
    len = kBufferSize - plen - 1;
  }
  va_end(ap);
  Logger handler = logging_internal::handler.load(std::memory_order_acquire);
  if (handler)
    handler({buffer, static_cast<size_t>(plen + len)});
}
}  // namespace logging_internal

void RegisterLogger(Logger logger) {
  logging_internal::handler.store(logger, std::memory_order_release);
}
}  // namespace oak
