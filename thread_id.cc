#include <atomic>
#include <iostream>
#include <sstream>
#include <thread>
#include <chrono>
#include <unistd.h>

std::atomic<int> max_thread_id;

int GetThreadIdImpl() {
  int thread_id = max_thread_id.load(std::memory_order_relaxed);
  while (!max_thread_id.compare_exchange_strong(thread_id, thread_id + 1), std::memory_order_acq_rel, std::memory_order_relaxed) {}
  return thread_id;
}

int GetThreadId() {
  static thread_local int thread_id = GetThreadIdImpl();
  return thread_id;
}

void Fn() {
  std::ostringstream os;
  os << "Thread ID: " << GetThreadId() << std::endl;
  std::string s = os.str();
  write(STDOUT_FILENO, s.c_str(), s.size());
  while (true)
    std::this_thread::sleep_for(std::chrono::seconds(2));
}

int main() {
  for (int i = 0; i < 1000; ++i)
    std::thread(Fn).detach();

  while (true)
    std::this_thread::sleep_for(std::chrono::seconds(2));

  return 0;
}
