// Copyright 2022 The Oak Authors.

#ifndef OAK_COMMON_DEBUG_H_
#define OAK_COMMON_DEBUG_H_

#include <stddef.h>

namespace oak {
// Signale alternate stack.

// Checks whether or not the signal alternate stack has been enabled.
bool SignalAltStackEnabled();

// Setup signal alternate stack, there is safety for multiple calls.
bool SetupSignalAltStack();

// FailureSignalLogger

// This class will be called upon failure with a string argument
// containing failure message and the number of bytes. This class
// may used write failure message to a secondary location, such as
// log file. The FailureSignalLogger.Log function runs within signal
// handler, it should be async signal safety.
class FailureMessageWriter {
 public:
  FailureMessageWriter();
  virtual ~FailureMessageWriter();

  virtual void Write(const char* msg, size_t size) = 0;
};

void RegisterFailureMessageWriter(FailureMessageWriter* writer);

// Register a failure signal message logger for common failure signal
// SIGSEGV, SIGILL, SIGFPE, SIGBUS, SIGABRT. The failure signal writes
// process failure message useful for debugging, if unspecified writer,
// the failure message will be emitted to the STDERR.
void RegisterFailureSignalHandler();
}  // namespace oak

#endif  // OAK_COMMON_DEBUG_H_
