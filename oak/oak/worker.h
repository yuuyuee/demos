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
#include "oak/common/macros.h"
#include "oak/common/system.h"
#include "oak/common/channel.h"
#include "oak/addons/module.h"
#include "oak/addons/source_handle.h"
#include "oak/addons/parser_handle.h"
#include "oak/addons/sink_handle.h"

namespace oak {
#define OAK_THREAD_MAX 128

enum ThreadState {
  THREAD_INIT,
  THREAD_RUNNING,
  THREAD_STOP
};

struct alignas(OAK_CACHELINE_SIZE) SinkContext {
  int id{0};
  std::atomic<int> state;

  pthread_t self;
  std::vector<const LogicCore*> cores;
  SinkConfig* config;

  // below as thread maintenance parameters
  std::unique_ptr<Channel> stream;

  std::vector<std::unique_ptr<SinkHandle>> handles;
};

struct Task {
  size_t task_id{0};
  int task_type{0};
  size_t protocol_type{0};

  size_t input_flow{0};
  size_t output_data{0};
  size_t keep_flow{0};
};

struct alignas(OAK_CACHELINE_SIZE) ParserContext {
  int id{0};
  std::atomic<int> state;

  pthread_t self;
  std::vector<const LogicCore*> cores;
  ParserConfig* config;

  size_t report_interval{60};

  SinkContext* sink_ref[OAK_THREAD_MAX];
  size_t num_sink{0};
  size_t offset{0};

  // below as thread maintenance parameters
  std::unique_ptr<Channel> stream;
  std::unique_ptr<Channel> report;

  std::mutex lock;
  std::unordered_map<uint64_t, std::unique_ptr<ParserHandle>> handles;
  std::unordered_map<uint64_t, Task> tasks;
};

struct alignas(OAK_CACHELINE_SIZE) SourceContext {
  int id{0};
  std::atomic<int> state;

  pthread_t self;
  std::vector<const LogicCore*> cores;
  SourceConfig* config;

  ParserContext* parser_ref[OAK_THREAD_MAX];
  size_t num_parser{0};
  size_t offset{0};

  // below as thread maintenance parameters
  std::unique_ptr<SourceHandle> handle;
};

}  // namespace oak

#endif  // OAK_WORKER_H_

