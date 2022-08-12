// Copyright 2022 The Oak Authors.

#include <stdio.h>

#include "oak/addons/public/compiler.h"
#include "oak/addons/public/platform.h"
#include "oak/addons/public/version.h"

namespace oak {
void Master(int argc, char* argv[]);
void Worker(int argc, char* argv[]);
}  // namespace oak

#define OAK_LINE_STR(x) x "\n"
#define OAK_RS_USAGE                                              \
  OAK_LINE_STR("Usage: rs <-w> ...")                              \
  OAK_LINE_STR("Version: " OAK_VERSION_STR)                       \
  OAK_LINE_STR("Options:")                                        \
  OAK_LINE_STR("    -w running as woker, default as master")      \
  OAK_LINE_STR("    -h print help")

int main(int argc, char* argv[]) {
  bool running_as_master = true;
  for (int i = 1; i < argc; ++i) {
    if (argv[i][0] == '-' &&
        argv[i][1] == 'h' &&
        argv[i][2] == '\0') {
      printf("%s", OAK_RS_USAGE);
      return 0;
    } else if (argv[i][0] == '-' &&
               argv[i][1] == 'w' &&
               argv[i][2] == '\0') {
      running_as_master = false;
    }
  }
  running_as_master ? oak::Master(argc, argv)
                    : oak::Worker(argc, argv);
  return 0;
}
