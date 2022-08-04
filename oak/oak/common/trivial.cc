// Copyright 2022 The Oak Authors.

#include "oak/common/trivial.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <limits.h>
#include <libgen.h>

#include <string>

#include "oak/logging/logging.h"

namespace oak {
void ChangeToBinDirectory(const char* argv0) {
  const char* p = strrchr(argv0, '/');
  std::string path = p ? std::string(argv0, p - argv0) : ".";
  int ret = chdir(path.c_str());
  if (!ret)
    OAK_FATAL("Unable to change working directory: %s\n", strerror(errno));
}

bool AlreadyRunning(const char* pid_name) {
  constexpr const int kSelfSize = 128;
  char self[kSelfSize];
  int self_len = snprintf(self, kSelfSize, "%d", static_cast<int>(getpid()));
  mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH;
  int fd = open(pid_name, O_WRONLY | O_CREAT, mode);
  if (fd < 0)
    OAK_FATAL("can't open file %s: %s\n", pid_name, strerror(errno));
  int ret = flock(fd, LOCK_EX | LOCK_NB);
  if (ret < 0) {
    if (errno == EWOULDBLOCK)
      return true;
    OAK_FATAL("can't lock file %s: %s\n", pid_name, strerror(errno));
  }
  ret = write(fd, self, self_len);
  if (ret != self_len) {
    const char* err = ret < 0 ? strerror(errno) : "unknown";
    OAK_FATAL("Write file %s expect written %d but written %d: %s\n",
              pid_name, self_len, ret < 0 ? 0 : ret, err);
  }
  return false;
}
}  // namespace oak
