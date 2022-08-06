// Copyright 2022 The Oak Authors.

#ifndef OAK_COMMON_TRIVIAL_H_
#define OAK_COMMON_TRIVIAL_H_

#include <string>

namespace oak {
// Setup process name

// Locks pid file to prevent process run again.
bool AlreadyRunning(const std::string& lock_file);
}  // namespace oak

#endif  // OAK_COMMON_TRIVIAL_H_
