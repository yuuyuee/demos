// Copyright 2022 The Oak Authors.

#include "oak/common/trivial.h"

#include <unistd.h>
#include "oak/common/format.h"
#include "oak/common/fs.h"

namespace oak {

bool AlreadyRunning(const std::string& lock_file) {
  char pid[32];
  int len = format(pid, sizeof(pid), "%d", getpid());
  File file = File::MakeWritableFile(lock_file);
  if (!file.TryLock()) {
    // Other process has already locked.
    return true;
  }
  file.Write(pid, len);
  // Note: releases and no longger owned the file object because
  // there should to keep locking.
  file.Release();
  return false;
}

}  // namespace oak
