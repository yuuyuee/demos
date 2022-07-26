// Copyright 2022 The Oak Authors.

#ifndef LOGGING_INTERNAL_LOGGING_H_
#define LOGGING_INTERNAL_LOGGING_H_

#include "common/macros.h"
#include "logging/log_level.h"

// This is potentially useful for internal logging and assertions,
// using INTERNAL LOGGING neither for its async-signal-safety nor for
// its non-allocating nature, but rather because RAW INTERNAL has
// very few other dependencies.

// Example:
//    OAK_SET_INTERNAL_LOG_LEVEL(DEBUG);
//    OAK_INTERNAL_LOG(DEBUG, "something errors");

#define OAK_SET_INTERNAL_LOG_LEVEL(level)                         \
  oak::internal_logging_internal::GetInternalLogLevel(            \
      oak::LogLevel::OAK_LOG_LEVEL_ ## level)

#define OAK_INTERNAL_LOG(level, ...)  do {                        \
  oak::internal_logging_internal::InternalLog(                    \
      oak::StrLogLevel(oak::LogLevel::OAK_LOG_LEVEL_ ## level),   \
      oak::internal_logging_internal::Basename(                   \
          __FILE__, sizeof(__FILE__) - 1),                        \
      __LINE__, __VA_ARGS__);                                     \
                                                                  \
  if (oak::LogLevel::OAK_LOG_LEVEL_ ## level                      \
      == oak::LogLevel::OAK_LOG_LEVEL_FATAL)                      \
    OAK_UNREACHABLE_CODE();                                       \
} while (0)

#define OAK_INTERNAL_LOG_DEBUG(...)   OAK_INTERNAL_LOG(DEBUG, __VA_ARGS__)
#define OAK_INTERNAL_LOG_INFO(...)    OAK_INTERNAL_LOG(INFO, __VA_ARGS__)
#define OAK_INTERNAL_LOG_WARNING(...) OAK_INTERNAL_LOG(WARNING, __VA_ARGS__)
#define OAK_INTERNAL_LOG_ERROR(...)   OAK_INTERNAL_LOG(ERROR, __VA_ARGS__)
#define OAK_INTERNAL_LOG_FATAL(...)   OAK_INTERNAL_LOG(FATAL, __VA_ARGS__)

#define OAK_INTERNAL_ASSERT(cond, msg) do {                       \
  if (OAK_EXPECT_FALSE(!(cond)))                                  \
    OAK_INTERNAL_LOG_FATAL("Assert " # cond "failed: %s\n", msg); \
} while (0)

namespace oak {
namespace internal_logging_internal {
constexpr const char* Basename(const char* fname, int off) {
  return off == 0 || fname[off - 1] == '/'
      ? fname + off : Basename(fname, off - 1);
}

void SetInternalLogLevel(oak::LogLevel level);
void InternalLog(const char* fname, int line, const char* fmt, ...);
}  // namespace internal_logging_internal
}  // namespace oak

#endif  // LOGGING_INTERNAL_LOGGING_H_
