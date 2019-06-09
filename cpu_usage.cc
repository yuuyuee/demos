#include <unistd.h>
#include <pthread.h>
#include <sched.h>
#include <iostream>
#include <string>
#include <chrono>
#include <ctime>
#include <thread>
#include <mutex>
#include <cassert>

struct cpu_info {
  size_t user;
  size_t nice;
  size_t system;
  size_t idle;
  size_t io_wait;
  size_t irq;
  size_t soft_irq;
  size_t steal;
  size_t guest;
  size_t guest_nice;
};

static bool run = false;
void Payload() {
  static unsigned int val = 1;
  while (true) {
    if (run) {
      val += 10;
    } else {
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }
  }
}

void SetAffinity(std::thread& td, int core) {
  cpu_set_t set;
  CPU_ZERO(&set);
  assert(core >= 0 && core <= 2);
  CPU_SET(core, &set);
  auto ret = pthread_setaffinity_np(td.native_handle(), sizeof(set), &set);
  assert(ret == 0);
}

void SetAffinity(pid_t pid, int core) {
  cpu_set_t set;
  CPU_ZERO(&set);
  assert(core >= 0 && core <= 2);
  CPU_SET(core, &set);
  auto ret = sched_setaffinity(pid, sizeof(set), &set);
  assert(ret == 0);
}

void PayloadThread() {
  std::thread td(Payload);
  SetAffinity(td, 1);
  td.detach();
}

int main() {
  SetAffinity(getpid(), 2);
  PayloadThread();

  while (true) {
    run = !run;
    std::this_thread::sleep_for(std::chrono::seconds(5));
  }

  return 0;
}
