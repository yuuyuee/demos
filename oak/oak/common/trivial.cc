// Copyright 2022 The Oak Authors.

#include "oak/common/trivial.h"

#include <sched.h>
#include <sys/syscall.h>
#include <sys/prctl.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <atomic>
#include "oak/common/format.h"
#include "oak/common/throw_delegate.h"

namespace oak {

pid_t System::GetThreadId() {
  return syscall(SYS_gettid);
}

pid_t System::GetCachedThreadId() {
  static thread_local pid_t thread_id = GetThreadId();
  return thread_id;
}

namespace {
std::atomic<pid_t> max_logic_thread_id;
pid_t GetLogicThreadIdImpl() {
  pid_t thread_id = max_logic_thread_id.load(std::memory_order_relaxed);
  while (!max_logic_thread_id
      .compare_exchange_strong(
          thread_id, thread_id + 1,
          std::memory_order_acq_rel,
          std::memory_order_relaxed)) {}
  return thread_id;
}
}  // anonymous namespace

pid_t System::GetLogicThreadId() {
  static thread_local pid_t thread_id = GetLogicThreadIdImpl();
  return thread_id;
}

int System::GeCurrenttCpu() {
  return sched_getcpu();
}

void System::ThreadYield() {
  sched_yield();
}

void System::SetupParentDeathSignal(int signo) {
  int ret = prctl(PR_SET_PDEATHSIG, signo);
  if (ret < 0) {
    ThrowStdSystemError(
        Format("prctl(PR_SET_PDEATHSIG, %d) failed: signo, %s",
               signo, strerror(errno)));
  }
}

}  // namespace oak
