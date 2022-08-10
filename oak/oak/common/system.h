// Copyright 2022 The Oak Authors.

#ifndef OAK_COMMON_SYSTEM_H_
#define OAK_COMMON_SYSTEM_H_

#include <unistd.h>
#include <pthread.h>

#include <string>
#include <functional>

namespace oak {

#define OAK_MAX_NUMA_NODES (8)
#define OAK_MAX_LOGIC_CORES (128)

struct SocketNode {
  int enable;
  int socket_id;
  int logic_core_id[OAK_MAX_LOGIC_CORES];
};

struct LogicCore {
  bool enable;
  bool lock;
  int logic_core_id;
  int core_id;
  cpu_set_t mask;
  int socket_id[OAK_MAX_NUMA_NODES];
};

struct CpuLayout {
  SocketNode socket_node[OAK_MAX_NUMA_NODES];
  LogicCore logic_core[OAK_MAX_LOGIC_CORES];
};


struct ThreadArgs {
  std::string& name;
  cpu_set_t favor;
  std::function<void()> routine;
};

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
  static void SetParentDeathSignal(int signo);

  // Initialize current CPU layout, this is not thread safe and it
  // is only calling before any thread has been created.
  static void InitCpuLayout(CpuLayout* cpu_layout);

  // Create a new thread named @name, starts execution by invoking
  // @routine running in core @favor. if @name is empty meaning that
  // do not setting the name for the new thread. if @favor is empty
  // meaning that do not setting the CPU affinity for the new thread.
  static void CreateThread(const ThreadArgs& param);

  // Setup the thread affinity of the CPU.
  static void SetPthreadAffinity(pthread_t id, const cpu_set_t& mask);
  static void SetThreadAffinity(pid_t id, const cpu_set_t& mask);
};

}  // namespace oak

#endif  // OAK_COMMON_SYSTEM_H_