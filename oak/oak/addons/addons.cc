// Copyright 2022 The Oak Authors.

#include "oak/addons/addons.h"

#include <unistd.h>

namespace oak {


void SourceRoutine(SourceContext* context) {
  // TODO(YUYUE):

  while (true)
    sleep(2);
}

void ParserRoutine(ParserContext* context) {
  // TODO(YUYUE):

  while (true)
    sleep(2);
}

void SinkRoutine(SinkContext* context) {
  // TODO(YUYUE):

  while (true)
    sleep(2);
}

}  // namespace oak
