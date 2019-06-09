#include <iostream>
#include <string>
#include <system_error>
#include <experimental/filesystem>

int main() {
  throw std::experimental::filesystem::filesystem_error("check failed.", "path1", "path2", std::error_code(EINVAL, std::system_category()));
  return 0;
}
