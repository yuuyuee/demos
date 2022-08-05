// Copyright 2022 The Oak Authors.

#include <sys/types.h>
#include <unistd.h>

#include "oak/addons/public/compiler.h"
#include "oak/addons/public/platform.h"
#include "oak/common/trivial.h"
#include "oak/logging/logging.h"
#include "oak/config.h"

int main(int argc, char* argv[]) {
  assert(argc || true);
  std::string self = oak::DirectoryName(argv[0]);
  oak::ChangeWorkDirectory(self.c_str());
  OAK_DEBUG("Change working directory to %s\n",
            oak::GetCurrentDirectory().c_str());

  // if (oak::AlreadyRunning(PROGRAM_NAME)) {
  //   OAK_ERROR("Already running.\n");
  //   return 0;
  // }

  oak::ProcessConfig proc_config;
  oak::InitProcessConfig(&proc_config);

  oak::MasterConfig master_config;
  std::string fname = proc_config.etc_dir + "/setup.json";
  oak::InitMasterConfig(&master_config, fname);

  // Logger handler = {};
  // RegisterLogger(handler);

  // waitting for slaver is running
  // entry event loop to waitting for task event

  while (true)
    sleep(2);
  return 0;
}
