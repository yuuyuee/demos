// Copyright 2022 The Oak Authors.

#ifndef LOGGING_LOGGING_H_
#define LOGGING_LOGGING_H_

#include "common/macros.h"
#include "logging/log_level.h"

// OAK_RAW_LOG
//
// OAK_RAW_LOG is are useful to call at any point during initialization,
// they do not allocate memory or acquire locks, and are async-signal
// safe and very few other dependencies.

#define OAK_RAW_LOG(level, ...)  do {                               \
  oak::logging_internal::RawLog(                                    \
      oak::LogLevelShortName(                                       \
          oak::LogLevel::OAK_LOG_LEVEL_ ## level),                  \
      oak::logging_internal::Basename(                              \
          __FILE__, sizeof(__FILE__) - 1),                          \
      __LINE__, __VA_ARGS__);                                       \
                                                                    \
  if (oak::LogLevel::OAK_LOG_LEVEL_ ## level                        \
      == oak::LogLevel::OAK_LOG_LEVEL_FATAL)                        \
    OAK_UNREACHABLE_CODE();                                         \
} while (0)

#define OAK_RAW_LOG_DEBUG(...)   OAK_RAW_LOG(DEBUG, __VA_ARGS__)
#define OAK_RAW_LOG_INFO(...)    OAK_RAW_LOG(INFO, __VA_ARGS__)
#define OAK_RAW_LOG_WARNING(...) OAK_RAW_LOG(WARNING, __VA_ARGS__)
#define OAK_RAW_LOG_ERROR(...)   OAK_RAW_LOG(ERROR, __VA_ARGS__)
#define OAK_RAW_LOG_FATAL(...)   OAK_RAW_LOG(FATAL, __VA_ARGS__)

#define OAK_RAW_CHECK(cond, ...) do {                               \
  if (OAK_EXPECT_FALSE(!(cond)))                                    \
    OAK_RAW_LOG_FATAL("Check " # cond "failed: " __VA_ARGS__);      \
} while (0)

namespace oak {
namespace logging_internal {

// Strip directory which any leading directory compoents has removed.
constexpr const char* Basename(const char* fname, int off) {
  return off == 0 || fname[off - 1] == '/'
      ? fname + off : Basename(fname, off - 1);
}

// OAK_RAW_LOG implement which does not allocate memory or acquire locks.
void RawLog(const char* fname, int line, char level, const char* fmt, ...)
    OAK_ATTR_PRINTF(4, 5);
}  // namespace logging_internal
}  // namespace oak

#endif  // LOGGING_LOGGING_H_
