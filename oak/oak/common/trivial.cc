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
#include <assert.h>

#include "oak/logging/logging.h"

namespace oak {

std::string DirectoryName(const char* path) {
  std::string real_path = GetRealPath(path);
  assert(!real_path.empty() && "Invalid path");
  auto pos = real_path.rfind('/');
  return pos == 0 ? "/" : real_path.substr(0, pos);
}

std::string GetCurrentDirectory() {
  char path[PATH_MAX];
  const char* p = getcwd(path, PATH_MAX);
  if (!p)
    OAK_FATAL("GetCurrentDirectory() failed: %s\n", strerror(errno));
  return std::string(path);
}

std::string GetRealPath(const char* path) {
  assert(path && "Invalid path");
  assert(path && "Invalid real_path");
  char real_path[PATH_MAX];
  const char* p = realpath(path, real_path);
  if (!p)
    OAK_FATAL("Realpath() failed: %s\n", strerror(errno));
  return std::string(real_path);
}

void CreateDirectoryRecursively(const char* dir_name) {
  assert(dir_name && "Invalid directory name");
  static unsigned int mode = S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH;
  int ret = mkdir(dir_name, mode);
  if (ret < 0) {
    if (errno == EEXIST) {
      // XXX: dir_name already exists but not necessarily as a directory.
      return;
    } else if (errno == ENOENT) {
      std::string path = DirectoryName(dir_name);
      CreateDirectoryRecursively(path.c_str());
      ret = mkdir(dir_name, mode);
      if (ret < 0)
        OAK_FATAL("CreateDirectory() failed: %s\n", strerror(errno));
    } else {
      OAK_FATAL("CreateDirectory() failed: %s\n", strerror(errno));
    }
  }
}

void ChangeWorkDirectory(const char* path) {;
  int ret = chdir(path);
  if (ret < 0)
    OAK_FATAL("ChangeWorkingDirectory() failed: %s\n", strerror(errno));
}

bool AlreadyRunning(const char* pid_fname) {
  constexpr const int kSelfSize = 128;
  char pid[kSelfSize];
  int pid_len = snprintf(pid, kSelfSize, "%d", getpid());
  mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH;
  int fd = open(pid_fname, O_WRONLY | O_CREAT, mode);
  if (fd < 0)
    OAK_FATAL("Can not open file %s: %s\n", pid_fname, strerror(errno));
  int ret = flock(fd, LOCK_EX | LOCK_NB);
  if (ret < 0) {
    if (errno == EWOULDBLOCK)
      return true;
    OAK_FATAL("Can not lock file %s: %s\n", pid_fname, strerror(errno));
  }
  ret = write(fd, pid, pid_len);
  if (ret != pid_len) {
    const char* err = ret < 0 ? strerror(errno) : "unknown";
    OAK_FATAL("Write file %s expect written %d but written %d: %s\n",
              pid_fname, pid_len, ret < 0 ? 0 : ret, err);
  }
  return false;
}
}  // namespace oak
