// Copyright 2022 The Oak Authors.

#include "oak/common/trivial.h"

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

#include "oak/logging/logging.h"

namespace oak {
bool AlreadyRunning(const char* pname) {
  constexpr const int kSelfSize = 128;
  char self[kSelfSize];
  int self_len = snprintf(self, kSelfSize, "%d", static_cast<int>(getpid()));
  mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH;
  char guard_fname[NAME_MAX];
  snprintf(guard_fname, NAME_MAX, "%s.pid", pname);
  int fd = open(guard_fname, O_WRONLY | O_CREAT, mode);
  if (fd < 0)
    OAK_FATAL("can't open file %s: %s\n", guard_fname, strerror(errno));
  int ret = flock(fd, LOCK_EX | LOCK_NB);
  if (ret < 0) {
    if (errno == EWOULDBLOCK)
      return true;
    OAK_FATAL("can't lock file %s: %s\n", guard_fname, strerror(errno));
  }
  ret = write(fd, self, self_len);
  if (ret != self_len) {
    const char* err = ret < 0 ? strerror(errno) : "unknown";
    OAK_FATAL("Write file %s expect written %d but written %d: %s\n",
              guard_fname, self_len, ret < 0 ? 0 : ret, err);
  }
  return false;
}
}  // namespace oak
