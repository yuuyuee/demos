// Copyright 2022 The Oak Authors.

#include "oak/common/fs.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>

#include "oak/logging/logging.h"

namespace oak {

std::string DirectoryName(const std::string& path) {
  assert(!path.empty() && "Invalid path");
  std::string real_path = GetRealPath(path);
  auto pos = real_path.rfind('/');
  return pos == 0 ? "/" : real_path.substr(0, pos);
}

std::string GetCurrentDirectory() {
  char path[PATH_MAX];
  const char* p = getcwd(path, PATH_MAX);
  if (!p)
    OAK_FATAL("Get current directory failed: %s\n", strerror(errno));
  return std::string(path);
}

std::string GetRealPath(const std::string& path) {
  assert(!path.empty() && "Invalid path");
  char real_path[PATH_MAX];
  const char* p = realpath(path.c_str(), real_path);
  if (!p) {
    OAK_FATAL("Get real path from \'%s\'failed: %s\n",
              path.c_str(), strerror(errno));
  }
  return std::string(real_path);
}

void CreateDirectoryRecursively(const std::string& directory) {
  assert(!directory.empty() && "Invalid directory name");
  static unsigned int mode = S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH;
  int ret = mkdir(directory.c_str(), mode);
  if (ret < 0) {
    if (errno == EEXIST) {
      // XXX: directory already exists but not necessarily as a directory.
      return;
    } else if (errno == ENOENT) {
      std::string path = DirectoryName(directory);
      CreateDirectoryRecursively(path);
      ret = mkdir(directory.c_str(), mode);
      if (ret < 0) {
        OAK_FATAL("Creates directory \'%s\' failed: %s\n",
                  directory.c_str(), strerror(errno));
      }
    } else {
      OAK_FATAL("Creates directory \'%s\' failed: %s\n",
                directory.c_str(), strerror(errno));
    }
  }
}

void ChangeWorkDirectory(const std::string& directory) {;
  int ret = chdir(directory.c_str());
  if (ret < 0) {
    OAK_FATAL("Changes work directory to \'%s\'failed: %s\n",
              directory.c_str(), strerror(errno));
  }
}

void ReadFile(const std::string& path, std::string* buffer) {
  int fd = open(path.c_str(), O_RDONLY);
  if (fd < 0) {
    OAK_FATAL("Open file \'%s\' failed: %s\n",
              path.c_str(), strerror(errno));
  }

  std::string result;
  char tmp[1024];
  ssize_t n = 0;

  while ((n = read(fd, tmp, 1024)) != 0) {
    if (n < 0 && errno != EINTR) {
      OAK_FATAL("Read file \'%s\' failed: %s\n",
                path.c_str(), strerror(errno));
    } else {
      result.append(tmp, n);
    }
  }
  buffer->swap(result);
}

namespace {
void WriteFileImpl(const std::string& path, const std::string& buffer, int flag) {
  mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH;

  int fd = open(path.c_str(), flag, mode);
  if (fd < 0) {
    OAK_FATAL("Open file \'%s\' failed: %s\n",
              path.c_str(), strerror(errno));
  }

  ssize_t n = 0;
  do {
    n = write(fd, buffer.c_str(), buffer.size());
  } while (n < 0 && errno == EINTR);

  while (n < static_cast<ssize_t>(buffer.size())) {
    ssize_t ret = write(fd, buffer.c_str() + n, buffer.size() - n);
    if (ret < 0) {
      if (errno != EINTR) {
        OAK_FATAL("Read file \'%s\' failed: %s\n",
                  path.c_str(), strerror(errno));
      }
      ret = 0;
    }
    n += ret;
  }
}

}  // anonymous namespace

void WriteFile(const std::string& path, const std::string& buffer) {
  WriteFileImpl(path, buffer, O_WRONLY | O_CREAT | O_TRUNC);
}

void AppendFile(const std::string& path, const std::string& buffer) {
  WriteFileImpl(path, buffer, O_WRONLY | O_CREAT | O_APPEND);
}

}  // namespace oak
