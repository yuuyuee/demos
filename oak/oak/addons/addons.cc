// Copyright 2022 The Oak Authors.

#include "oak/addons/addons.h"

#include <unistd.h>

namespace oak {

void SourceRoutine(RoutineContext* context) {
  // TODO(YUYUE):

  while (true)
    sleep(2);
}

void ParserRoutine(RoutineContext* context) {
  // TODO(YUYUE):

  while (true)
    sleep(2);
}

void SinkRoutine(RoutineContext* context) {
  // TODO(YUYUE):

  while (true)
    sleep(2);
}

}  // namespace oak
