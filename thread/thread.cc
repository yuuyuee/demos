#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <unistd.h>
#include <sys/types.h>
#include <chrono>
#include <sys/syscall.h>

#if 0
int Func(int in) {
  in *= 10;
  return in;
}

int main(void) {
  //std::future<int> future = std::async(std::launch::async, Func, 10);
  //std::cout << future.get() << std::endl;
  std::packaged_task<int(int)> tasks(Func);
  auto future = tasks.get_future();
  tasks(100);
  future.wait();
  std::cout << future.get() << std::endl;
  return 0;
}
#endif

void Func() {
  while (true) {
    long int tid = syscall(__NR_gettid);
    std::cout << "On thread " << tid << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));
  }
}

int main() {
  std::thread t0(Func), t1(Func);
  t0.join();
  t1.join();
  return 0;
}
