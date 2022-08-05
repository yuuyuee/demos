// Copyright 2022 The Oak Authors.

#ifndef OAK_COMMON_TRIVIAL_H_
#define OAK_COMMON_TRIVIAL_H_

#include <string>

namespace oak {
// Return directory name of the path.
std::string DirectoryName(const char* path);

// Return current working directory.
std::string GetCurrentDirectory();

// Return the canonicalized absolute path anme.
std::string GetRealPath(const char* path);

// Creates an directory recursively.
void CreateDirectoryRecursively(const char* dir_name);

// Changes current working directory to path where @argv0 is inside.
void ChangeWorkDirectory(const char* argv0);

// Locks pid file to prevent process run again.
bool AlreadyRunning(const char* pid_name);
}  // namespace oak

#endif  // OAK_COMMON_TRIVIAL_H_
