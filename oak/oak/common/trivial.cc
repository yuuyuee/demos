// Copyright 2022 The Oak Authors.

#include "oak/common/trivial.h"

#include <sys/syscall.h>
#include <sched.h>
#include <atomic>

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

int GeCurrenttCpu() {
  return sched_getcpu();
}

void ThreadYield() {
  sched_yield();
}

}  // namespace oak
