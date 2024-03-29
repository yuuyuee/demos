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
#define OAK_THREAD_MAX 32

enum ThreadState {
  THREAD_INIT,
  THREAD_RUNNING,
  THREAD_STOP
};

struct alignas(OAK_CACHELINE_SIZE) SinkContext {
  int id{0};
  std::atomic<int> state;

  pthread_t self;
  std::vector<LogicCore> cores;
  const SinkConfig* config;

  // below as thread maintenance parameters
  std::unique_ptr<Channel> stream;

  std::vector<std::unique_ptr<SinkHandle>> handles;
};

using ParserHandleDict =
    std::unordered_map<uint64_t, std::unique_ptr<ParserHandle>>;

struct ProtocolTag {
  int64_t task_id{-1};
  uint64_t app_type{0};

  size_t input_flow{0};
  size_t output_data{0};
};

using ProtocolTagDict = std::unordered_map<uint64_t, ProtocolTag>;

struct alignas(OAK_CACHELINE_SIZE) ParserContext {
  int id{0};
  std::atomic<int> state;

  pthread_t self;
  std::vector<LogicCore> cores;
  const ParserConfig* config;

  size_t report_interval{60};

  SinkContext* sink_ref[OAK_THREAD_MAX];
  int num_sink{0};
  int offset{0};

  // below as thread maintenance parameters
  std::unique_ptr<Channel> stream;
  std::unique_ptr<Channel> report;

  std::mutex lock;
  ParserHandleDict handles;
  ProtocolTagDict tags;
};

struct alignas(OAK_CACHELINE_SIZE) SourceContext {
  int id{0};
  std::atomic<int> state;

  pthread_t self;
  std::vector<LogicCore> cores;
  const SourceConfig* config;

  ParserContext* parser_ref[OAK_THREAD_MAX];
  int num_parser{0};
  int offset{0};

  // below as thread maintenance parameters
  std::unique_ptr<SourceHandle> handle;
};

struct RuntimeEnviron {
  SourceContext source_context[OAK_THREAD_MAX];
  int num_source_context;

  ParserContext parser_context[OAK_THREAD_MAX];
  int num_parser_context;

  SinkContext sink_context[OAK_THREAD_MAX];
  int num_sink_context;
};

void InitRuntimeEnviron(RuntimeEnviron* env, const Config& config);
void CreateWorker(RuntimeEnviron* env);
int UpdateParser(ParserContext* context, const ModuleConfig& module_config);
}  // namespace oak

#endif  // OAK_WORKER_H_

