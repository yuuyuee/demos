// Copyright 2022 The Oak Authors.

#ifndef OAK_COMMON_FS_H_
#define OAK_COMMON_FS_H_

#include <string>

namespace oak {

// Return directory name of the path.
// Note: require the @path named file does exist.
std::string DirectoryName(const std::string& path);

// Return current working directory.
std::string GetCurrentDirectory();

// Return the canonicalized absolute path anme.
// Note: require the @path named file does exist.
std::string GetRealPath(const std::string& path);

// Creates an directory recursively.
void CreateDirectoryRecursively(const std::string& directory);

// Changes current working directory to path where @path is inside.
void ChangeWorkDirectory(const std::string& directory);

// Read the documents from a file.
void ReadFile(const std::string& path, std::string* buffer);

// Write the documents to a file.
void WriteFile(const std::string& path, const std::string& buffer);

// Append the documents to a file.
void AppendFile(const std::string& path, const std::string& buffer);

}  // namespace oak

#endif  // OAK_COMMON_FS_H_
