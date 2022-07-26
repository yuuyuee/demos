// Copyright 2022 The Oak Authors.

#ifndef OAK_LOGGING_LOG_LEVEL_H_
#define OAK_LOGGING_LOG_LEVEL_H_

namespace oak {

enum class LogLevel: int {
  OAK_LOG_LEVEL_DEBUG = 0,
  OAK_LOG_LEVEL_INFO,
  OAK_LOG_LEVEL_WARNING,
  OAK_LOG_LEVEL_ERROR,
  OAK_LOG_LEVEL_FATAL
};

#define OAK_MAKE_LOG_LEVEL_OPS(type, ops)                 \
constexpr bool operator ops(type lhs, type rhs) {         \
  return static_cast<int>(lhs) ops static_cast<int>(rhs); \
}

OAK_MAKE_LOG_LEVEL_OPS(LogLevel, >)
OAK_MAKE_LOG_LEVEL_OPS(LogLevel, >=)
OAK_MAKE_LOG_LEVEL_OPS(LogLevel, <)
OAK_MAKE_LOG_LEVEL_OPS(LogLevel, <=)
OAK_MAKE_LOG_LEVEL_OPS(LogLevel, ==)
OAK_MAKE_LOG_LEVEL_OPS(LogLevel, !=)

constexpr const char* StrLogLevel(oak::LogLevel level) {
  return level == LogLevel::OAK_LOG_LEVEL_DEBUG
      ? "DEBUG"
      : level == LogLevel::OAK_LOG_LEVEL_INFO
          ? "INFO"
          : level == LogLevel::OAK_LOG_LEVEL_WARNING
              ? "WARNING"
              : level == LogLevel::OAK_LOG_LEVEL_ERROR
                  ? "ERROR"
                  : level == LogLevel::OAK_LOG_LEVEL_FATAL
                    ? "FATAL" : "UNKNOWN";
}

}  // namespace oak

#endif  // OAK_LOGGING_LOG_LEVEL_H_
