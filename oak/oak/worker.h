// Copyright 2022 The Oak Authors.

#ifndef OAK_WORKER_H_
#define OAK_WORKER_H_

#include <pthread.h>

#include <atomic>
#include <memory>
#include <vector>
#include <mutex>  // NOLINT
#include <unordered_map>

#include "oak/config.h"
#include "oak/common/system.h"
#include "oak/common/channel.h"
#include "oak/addons/module.h"
#include "oak/addons/source_handle.h"
#include "oak/addons/parser_handle.h"
#include "oak/addons/sink_handle.h"

namespace oak {
#define OAK_THREAD_MAX 128

enum ThreadStatus {
  THREAD_INIT,
  THREAD_RUNNING,
  THREAD_STOP
};

struct SinkContext {
  int id{0};
  std::atomic<int> status;

  pthread_t self;
  std::vector<const LogicCore*> cores;

  std::unique_ptr<Channel> stream;

  std::vector<std::unique_ptr<SinkHandle>> handles;
  std::vector<ModuleArguments> args;
};

struct ParserContext {
  int id{0};
  std::atomic<int> status;

  pthread_t self;
  std::vector<const LogicCore*> cores;

  SinkContext* sink_ref[OAK_THREAD_MAX];
  size_t num_sink{0};
  size_t offset{0};

  std::unique_ptr<Channel> stream;
  std::unique_ptr<Channel> report;

  std::mutex lock;
  std::unordered_map<int, std::unique_ptr<ParserHandle>> handles;
  std::unordered_map<int, ModuleArguments> args;
};

struct SourceContext {
  int id{0};
  std::atomic<int> status;

  pthread_t self;
  std::vector<const LogicCore*> cores;

  ParserContext* parser_ref[OAK_THREAD_MAX];
  size_t num_parser{0};
  size_t offset{0};

  std::vector<std::unique_ptr<SourceHandle>> handles;
  std::vector<ModuleArguments> args;
};

}  // namespace oak

#endif  // OAK_WORKER_H_

