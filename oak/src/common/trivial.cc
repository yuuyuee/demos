// Copyright 2022 The Oak Authors.

#include "common/trivial.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <errno.h>
#include <limits.h>

namespace oak {
namespace trivial_internal {
namespace {

constexpr const char kTailMsg[] = "... (message truncated)\n";
constexpr const size_t kTailMsgSize = sizeof(kTailMsg);
}  // anonymous namespace

void RawLog(const char* fname, int line, const char* fmt, ...) {
  constexpr const int kBufferSize = 2048;
  char buffer[kBufferSize];

  int plen = snprintf(buffer, kBufferSize, "P [%s:%d] ", fname, line);

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

  write(STDERR_FILENO, buffer, plen + len);
}

}  // namespace trivial_internal

bool AlreadyRunning(const char* pname) {
  constexpr const int kSelfSize = 128;
  char self[kSelfSize];
  int self_len = snprintf(self, kSelfSize, "%d", static_cast<int>(getpid()));
  mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH;
  char guard_fname[NAME_MAX];
  snprintf(guard_fname, NAME_MAX, ".%s.pid",
      trivial_internal::Basename(pname, strlen(pname)));
  int fd = open(guard_fname, O_WRONLY | O_CREAT, mode);
  if (fd < 0)
    OAK_PANIC("can't open file %s: %s\n", guard_fname, strerror(errno));
  int ret = flock(fd, LOCK_EX | LOCK_NB);
  if (ret < 0) {
    if (errno == EWOULDBLOCK)
      return true;
    OAK_PANIC("can't lock file %s: %s\n", guard_fname, strerror(errno));
  }
  ret = write(fd, self, self_len);
  if (ret != self_len) {
    const char* err = ret < 0 ? strerror(errno) : "unknown";
    OAK_PANIC("Write file %s expect written %d but written %d: %s\n",
              guard_fname, self_len, ret < 0 ? 0 : ret, err);
  }
  return false;
}
}  // namespace oak
