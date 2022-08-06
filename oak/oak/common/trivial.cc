// Copyright 2022 The Oak Authors.

#include "oak/common/trivial.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <limits.h>

#include "oak/logging/logging.h"

namespace oak {

bool AlreadyRunning(const char* pid_fname) {
  constexpr const int kSelfSize = 128;
  char pid[kSelfSize];

  int pid_len = snprintf(pid, kSelfSize, "%d", getpid());
  mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH;
  int fd = open(pid_fname, O_WRONLY | O_CREAT, mode);
  if (fd < 0)
    OAK_FATAL("Open file %s: %s\n", pid_fname, strerror(errno));

  int ret = flock(fd, LOCK_EX | LOCK_NB);
  if (ret < 0) {
    if (errno == EWOULDBLOCK)
      return true;
    OAK_FATAL("Lock file %s: %s\n", pid_fname, strerror(errno));
  }

  ssize_t n = 0;
  do {
    n = write(fd, pid, pid_len);
  } while (n < 0 && errno == EINTR);

  if (n < 0) {
    OAK_FATAL("Write file \'%s\' failed: %s\n",
              pid_fname, strerror(errno));
  }
  return false;
}

}  // namespace oak
