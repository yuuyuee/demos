// Copyright 2022 The Oak Authors.

#include "oak/addons/addons.h"

#include <unistd.h>

namespace oak {
static_assert(offsetof(ModuleHeader, priv_data) ==
              offsetof(oak_source_module, priv_data),
              "logic error");
static_assert(offsetof(ModuleHeader, priv_data) ==
              offsetof(oak_parser_module, priv_data),
              "logic error");
static_assert(offsetof(ModuleHeader, priv_data) ==
              offsetof(oak_sink_module, priv_data),
              "logic error");

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
