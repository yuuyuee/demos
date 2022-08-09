// Copyright 2022 The Oak Authors.

#ifndef OAK_COMMON_TRIVIAL_H_
#define OAK_COMMON_TRIVIAL_H_

#include <unistd.h>

namespace oak {

struct System {
  // Returns the native thread identification.
  static pid_t GetThreadId();

  // Like as GetThreadId() but not safe to call in an asynchronous
  // signal handling context nor right after a call to fork().
  static pid_t GetCachedThreadId();

  // Returns the logic thread identification within process. This
  // function is not safe to call in an asynchronous signal handling
  // content nor right after a call to fork().
  static pid_t GetLogicThreadId();

  // Returns CPU on which the calling thread is running.
  static int GeCurrenttCpu();

  // Yield the calling thread to relinquish the CPU.
  static void ThreadYield();

  // Setup the parent process death signal of the calling process.
  static void SetupParentDeathSignal(int signo);
};

}  // namespace oak

#endif  // OAK_COMMON_TRIVIAL_H_
