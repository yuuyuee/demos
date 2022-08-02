// Copyright 2022 The Oak Authors.

#include <sys/types.h>
#include <unistd.h>

#include <vector>
#include <string>
#include <initializer_list>
#include <algorithm>
#include <utility>

#include "oak/addons/internal/compiler.h"
#include "oak/addons/internal/platform.h"
#include "oak/common/trivial.h"
#include "oak/logging/logging.h"
#include "oak/config.h"

#define PROGRAM_NAME "master"

class Selector {
 public:
  explicit Selector(const char* fname): fname_(fname), path_() {}
  Selector& From(std::initializer_list<const char*> dirs);
  operator const char*() const noexcept;

 private:
  const char* fname_;
  std::vector<std::string> path_;
};

Selector& Selector::From(std::initializer_list<const char*> dirs) {
  for (auto& it : dirs) {
    std::string path(it);
    if (fname_) {
      if (path[path.size() - 1] != '/')
        path.push_back('/');
      path.append(fname_);
    }
    path_.push_back(std::move(path));
  }
  return *this;
}

Selector::operator const char*() const noexcept {
  auto it = std::find_if(path_.cbegin(), path_.cend(),
      [](const std::string& path) -> bool {
        return !access(path.c_str(), F_OK);
      });
  if (it != path_.cend())
    return it->c_str();
  return nullptr;
}

void CreateSlave(oak::Config&) {}
void EventLoop() {
  while (true) {
    sleep(2);
  }
}

// can not creating multiple processes with boot many process, because
// the number of processes is configured in configuration.

int main(int argc, char* argv[]) {
  if (oak::AlreadyRunning(PROGRAM_NAME)) {
    OAK_ERROR("Already running.\n");
    return 0;
  }

  oak::Config config;
  oak::LoadConfig(
      Selector{"setup.json"}
          .From({".", "../etc", "/etc"}),
      &config);

  // Logger handler = {};
  // RegisterLogger(handler);



  CreateSlave(config);

  EventLoop();
  return 0;
}
