#include <unistd.h>
#include <signal.h>
#include <thread>
#include <sstream>
#include <chrono>

void fn(int enable) {
  if (enable) {
    stack_t ss;
    ss.ss_flags = 0;
    ss.ss_size = SIGSTKSZ;
    ss.ss_sp = new char[ss.ss_size];
    sigaltstack(&ss, nullptr);
  }


  while (true) {
    std::this_thread::sleep_for(std::chrono::seconds(2));
    stack_t ss;
    sigaltstack(nullptr, &ss);
    bool disable = ss.ss_flags & SS_DISABLE;
    std::ostringstream os;
    os << std::this_thread::get_id()
      << ": real enable: " << (enable ? "TRUE" : "FALSE")
      << ", check enable: " << (!disable ? "TRUE": "FALSE")
      << std::endl;
    std::string s = os.str();
    write(STDOUT_FILENO, s.c_str(), s.size());
  }

}

int main() {

  for (int i = 0; i < 2; ++i)
    std::thread(fn, i).detach();
  while (true)
    std::this_thread::sleep_for(std::chrono::seconds(2));
  return 0;
}
