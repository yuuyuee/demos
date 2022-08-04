// Copyright 2022 The Oak Authors.

#ifndef OAK_COMMON_TRIVIAL_H_
#define OAK_COMMON_TRIVIAL_H_

namespace oak {
void ChangeWorkingDirectory(const char* argv0);

bool AlreadyRunning(const char* pid_name);
}  // namespace oak

#endif  // OAK_COMMON_TRIVIAL_H_
