// Copyright 2022 The Oak Authors.

#include "oak/common/debug.h"

#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <time.h>
#include <sys/mman.h>
#include <atomic>

#include "oak/common/trivial.h"
#include "oak/common/format.h"
#include "oak/common/throw_delegate.h"

namespace oak {
FailureMessageWriter::FailureMessageWriter() {}
FailureMessageWriter::~FailureMessageWriter() {}

namespace {
// Default failure message writer
class AsyncSignalSafeWriter: public FailureMessageWriter {
 public:
  AsyncSignalSafeWriter(): FailureMessageWriter() {}
  virtual ~AsyncSignalSafeWriter() {}

  virtual void Write(const char* msg, size_t size);
};

void AsyncSignalSafeWriter::Write(const char* msg, size_t size) {
  if (msg && size > 0)
    write(STDERR_FILENO, msg, size);
}

AsyncSignalSafeWriter default_writer;
std::atomic<FailureMessageWriter*> failed_msg_writer(&default_writer);

bool SetupSignalAltStackImpl() {
  stack_t ss;
  memset(&ss, 0, sizeof(ss));
  ss.ss_size = SIGSTKSZ;

// sanitizer instrumention require additional stack space.
#if defined(__SANITIZE_ADDRESS__) || \
    defined(__SANITIZE_THREAD__)  || \
    defined(__SANITIZE_MEMORY__)
  ss.ss_size *= 5;
#endif
  ss.ss_sp = mmap(nullptr, ss.ss_size,
                  PROT_READ | PROT_WRITE,
                  MAP_PRIVATE | MAP_ANONYMOUS | MAP_STACK,
                  -1, 0);
  if (ss.ss_sp == MAP_FAILED) {
    ThrowStdSystemError(
        Format("map() for alternate signal stack failed: %s",
               strerror(errno)));
  }
  if (sigaltstack(&ss, nullptr) < 0) {
    ThrowStdSystemError(
        Format("sigaltstack() failed: %s", strerror(errno)));
  }
  return true;
}

const int kFailureSignal[] = {
  SIGSEGV, SIGILL, SIGFPE, SIGBUS, SIGABRT
};

const char* StrSignal(int signo) {
  return signo == SIGSEGV ? "SIGSEGV"
       : signo == SIGILL ? "SIGILL"
       : signo == SIGFPE ? "SIGFPE"
       : signo == SIGBUS ? "SIGBUS"
       : signo == SIGABRT ? "SIGABRT"
       : "UNKNOWN";
}

class PipeToBuffer {
 public:
  PipeToBuffer(char* buffer, size_t size);

 private:
  int fd[2];
  char* buffer_;
  size_t size_;
};

void ResetSignalHandlerAndRaise(int signo) {
  signal(signo, SIG_DFL);
  raise(signo);
}

std::atomic<pid_t> failed_tid(0);

void FailureSignalHandler(int signo, siginfo_t* info, void*) {
  const pid_t this_tid = oak::System::GetThreadId();
  pid_t prev_tid = 0;
  if (!failed_tid.compare_exchange_strong(
        prev_tid, this_tid,
        std::memory_order_acq_rel, std::memory_order_relaxed)) {
    if (prev_tid != this_tid)
      return;
  }

  char msg[128];
  size_t len = format(msg, sizeof(msg),
                      "*** %s received at %ld, pid = %d, uid = %d ***\n",
                      StrSignal(signo),
                      static_cast<long int>(time(0)),  // NOLINT
                      info->si_pid, info->si_uid);
  FailureMessageWriter* writer =
      failed_msg_writer.load(std::memory_order_acquire);
  writer->Write(msg, len);
  ResetSignalHandlerAndRaise(signo);
}

void RegisterFailureSignalHandlerOnce(int signo) {
  struct sigaction act;
  memset(&act, 0, sizeof(act));
  sigemptyset(&act.sa_mask);
  act.sa_flags |= SA_SIGINFO | SA_NODEFER | SA_ONSTACK;
  act.sa_sigaction = FailureSignalHandler;
  if (sigaction(signo, &act, nullptr) < 0) {
    ThrowStdSystemError(
        Format("sigaction() failed: %s", strerror(errno)));
  }
}

}  // anonymous namespace

bool SignalAltStackEnabled() {
  stack_t ss;
  if (sigaltstack(nullptr, &ss) < 0) {
    ThrowStdSystemError(
        Format("sigaltstack() failed: %s", strerror(errno)));
  }
  return !(ss.ss_flags & SS_DISABLE);
}

bool SetupSignalAltStack() {
  static thread_local bool enable = SetupSignalAltStackImpl();
  return enable;
}

void RegisterFailureMessageWriter(FailureMessageWriter* writer) {
  if (writer)
    failed_msg_writer.store(writer, std::memory_order_release);
}

void RegisterFailureSignalHandler() {
  for (auto signo : kFailureSignal)
    RegisterFailureSignalHandlerOnce(signo);
}

}  // namespace oak
