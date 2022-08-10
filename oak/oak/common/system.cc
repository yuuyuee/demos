// Copyright 2022 The Oak Authors.

#include "oak/common/system.h"

#include <sched.h>
#include <sys/syscall.h>
#include <sys/prctl.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <assert.h>

#include <atomic>

#include "oak/common/fs.h"
#include "oak/common/error_code.h"
#include "oak/common/format.h"
#include "oak/common/throw_delegate.h"
#include "oak/common/debug.h"

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

void System::SetParentDeathSignal(int signo) {
  int ret = prctl(PR_SET_PDEATHSIG, signo);
  if (ret < 0) {
    ThrowStdSystemError(
        Format("prctl(PR_SET_PDEATHSIG, %d) failed: %s",
               signo, Strerror(errno)));
  }
}

namespace {
int StrToInt(const char* s, size_t n) {
  int ret = 0;
  for (size_t i = 0; i < n; ++i) {
    if (!(s[i] >= '0' && s[i] <= '9'))
      break;
    ret = ret * 10 + (s[i] - '0');
  }
  return ret;
}

void InitSocketNode(SocketNode* socket_node) {
  for (int i = 0; i < OAK_MAX_NUMA_NODES; ++i) {
    socket_node[i].enable = false;
    socket_node[i].socket_id = i;
    for (int j = 0; j < OAK_MAX_LOGIC_CORES; ++j)
      socket_node[i].logic_core_id[j] = -1;
  }
}

void InitLogicCore(LogicCore* logic_core) {
  for (int i = 0; i < OAK_MAX_LOGIC_CORES; ++i) {
    logic_core[i].enable = false;
    logic_core[i].lock = false;
    logic_core[i].logic_core_id = i;
    logic_core[i].core_id = 0;
    CPU_ZERO(&logic_core[i].mask);
    for (int j = 0; j < OAK_MAX_NUMA_NODES; ++j)
      logic_core[i].socket_id[j] = -1;
  }
}

}  // anonymous namespace

#define OAK_SYS_CPU_DIR  "/sys/devices/system/cpu/"
#define OAK_SYS_NODE_DIR "/sys/devices/system/node/"
#define OAK_SYS_CORE_ID "/topology/core_id"

void System::InitCpuLayout(CpuLayout* cpu_layout) {
  InitSocketNode(cpu_layout->socket_node);
  InitLogicCore(cpu_layout->logic_core);

  SocketNode* socket_node = cpu_layout->socket_node;
  for (int i = 0; i < OAK_MAX_NUMA_NODES; ++i) {
    std::string path = Format(OAK_SYS_NODE_DIR "node%d", i);
    if (!IsExists(path))
      continue;
    socket_node[i].enable = true;
    socket_node[i].socket_id = i;
    for (int j = 0; j < OAK_MAX_LOGIC_CORES; ++j) {
      path = Format(OAK_SYS_NODE_DIR "/node%d/cpu%d", i, j);
      if (!IsExists(path))
        continue;
      socket_node[i].logic_core_id[j] = j;
    }
  }

  LogicCore* logic_core = cpu_layout->logic_core;
  for (int i = 0; i < OAK_MAX_LOGIC_CORES; ++i) {
    std::string path = Format(OAK_SYS_CPU_DIR "cpu%d" OAK_SYS_CORE_ID, i);
    if (!IsExists(path))
      continue;
    logic_core[i].enable = true;
    logic_core[i].lock = false;
    logic_core[i].logic_core_id = i;
    char tmp[128];
    File file = File::MakeReadOnlyFile(path);
    size_t len = file.Read(tmp, 128);
    logic_core[i].core_id = StrToInt(tmp, len);
    CPU_SET(i, &logic_core[i].mask);

    for (int j = 0; j < OAK_MAX_NUMA_NODES; ++j) {
      path = Format(OAK_SYS_CPU_DIR "cpu%d/node%d", i, j);
      if (!IsExists(path))
        continue;
      logic_core[i].socket_id[j] = j;
    }
  }
}

namespace {
void* StartRoutine(void* thread_args) {
  SetSignalAltStack();

  const ThreadArgs* args = reinterpret_cast<ThreadArgs*>(thread_args);
  if (!args->name.empty())
    oak::System::SetThreadName(args->name);

  if (CPU_COUNT(&args->favor) > 0) {
    oak::System::SetPthreadAffinity(pthread_self(), args->favor);
    oak::System::ThreadYield();
  }

  args->routine();
  return 0;
}

#define PTHREAD_ERROR(err, msg)             \
  if (err != 0) {                           \
    ThrowStdSystemError(                    \
        Format(msg ": %s", Strerror(err))); \
  }

}  // anonymous namespace

void System::CreateThread(const ThreadArgs& thread_args) {
  assert(thread_args.routine && "Invalid argument");

  pthread_attr_t attr;
  int err = pthread_attr_init(&attr);
  PTHREAD_ERROR(err, "pthread_attr_init() failed");

  err = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
  PTHREAD_ERROR(err, "pthread_attr_setdetachstate() failed")

  pthread_t pid;
  err = pthread_create(&pid, &attr, StartRoutine,
      const_cast<void*>(static_cast<const void*>(&thread_args)));
  PTHREAD_ERROR(err, "pthread_create() failed");
}

void System::SetThreadAffinity(pthread_t id, const cpu_set_t& mask) {
  int err = pthread_setaffinity_np(id, sizeof(cpu_set_t), &mask);
  PTHREAD_ERROR(err, "pthread_setaffinity_np() failed");
}

void System::SetThreadName(pthread_t id, const std::string& name) {
  // The name can be up to 16 bytes long, including the terminating null
  // byte, if the length of the string exceed 16 bytes, the string is
  // silently truncated.
  size_t size = name.size() < 15 ? name.size() : 15;
  char buffer[16];
  memcpy(buffer, name.c_str(), size);
  buffer[size + 1] = '\0';
  int err = pthread_setname_np(id, buffer);
  PTHREAD_ERROR(err, "pthread_setname_np() failed");
}

void System::SaveArgument(int, char* []) {
  // TODO(YUYUE):
}

void System::SetProcessName(const std::string&) {
  // TODO(YUYUE):
}



}  // namespace oak
