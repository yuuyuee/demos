// Copyright 2022 The Oak Authors.

#include "oak/worker.h"

#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <string.h>

#include <string>
#include <utility>
#include <functional>

#include "oak/common/format.h"
#include "oak/common/throw_delegate.h"
#include "oak/logging/logging.h"
#include "oak/addons/metadata_internal.h"
#include "oak/addons/public/event_define.h"

using std::unique_ptr;
using std::vector;
using std::unordered_map;
using std::atomic;
using std::mutex;
using std::string;

using oak::Channel;

namespace oak {
namespace {

string HumanReadableCoreInfo(const vector<const LogicCore*>& cores) {
  string msg;
  for (auto const& it : cores) {
    if (!msg.empty())
      msg.append(", ");
    msg.append(Format("%d [%d]", it->logic_core_id, it->socket_id));
  }
  return msg;
}

void SourceThreadRoutine(SourceContext* context) {
  assert(context->state.load(std::memory_order_acquire) == THREAD_INIT);

  const string title =
      Format("Source thread %d (TID %d)", context->id, System::GetThreadId());

  // Initialize thread environment.
  for (auto const& it : context->config->modules) {
    if (!it.second.enable)
      continue;

    if (SourceHandleFactory(it.second, &context->handle) != 0) {
      ThrowStdRuntimeError(
          Format("%s: Open module %s failed.",
              title.c_str(), it.first.c_str()));
    }
  }

  if (!context->handle)
    OAK_WARNING("%s: No one module enabled\n", title.c_str());

  // Setup thread state.
  OAK_INFO("%s: Running on the core %s\n",
           title.c_str(), HumanReadableCoreInfo(context->cores).c_str());
  context->state.store(THREAD_RUNNING, std::memory_order_release);

  // Entry thread loop
  int error_count = 0;
  struct oak_metadata metadata;
  while (context->state.load(std::memory_order_relaxed) != THREAD_STOP) {
    oak_metadata_init(&metadata);

    if (!context->handle || context->handle->Read(&metadata) != 0) {
      if (error_count < 4)
        ++error_count;
      sleep(error_count);
      // XXX(YUYUE): There may be other better designs.
      oak_metadata_free(&metadata);
      continue;
    }
    error_count = 0;

    struct oak_metadata* copy = new struct oak_metadata;
    *copy = metadata;
    for (; context->offset < context->num_parser; ++context->offset) {
      auto parser = context->parser_ref[context->offset];

      // Skipping the offlined parser.
      if (parser->state.load(std::memory_order_acquire) != THREAD_RUNNING) {
        OAK_WARNING("Parser thread %d has offlined\n", parser->id);
        continue;
      }

      if (!parser->stream->Push(copy)) {
        OAK_WARNING("Parser thread %d stream channel is fulling\n", parser->id);
        continue;
      }

      // Push to parser sucessful, release metadata ownership.
      copy = nullptr;
      break;
    }

    if (context->offset >= context->num_parser)
      context->offset = 0;

    // No one parser has living, free metadata copy
    if (copy) {
      unique_ptr<struct oak_metadata> free(copy);
      oak_metadata_free(free.get());
    }
  }  // while ()

  context->handle.reset();
}

struct ParserMetrics {
  int64_t task_id{-1};

  size_t input_flow{0};
  size_t output_data{0};
};

using ParserMetricsDict = unordered_map<uint64_t, ParserMetrics>;

void ReportMetrics(unique_ptr<Channel>* channel, ParserMetricsDict* metrics) {
  for (auto const& it : *metrics) {
    struct outgoing_event* event = new struct outgoing_event;
    event->type = ET_METRICS;
    oak_dict_init(&event->args);

    string task_id_str = Format("%ld", it.second.task_id);
    oak_dict_add(&event->args,
                 OAK_TASK_ID, sizeof(OAK_TASK_ID),
                 task_id_str.c_str(), task_id_str.size());

    string input_flow_str = Format("%ld", it.second.input_flow);
    oak_dict_add(&event->args,
                 OAK_M_INPUT_FLOW, sizeof(OAK_M_INPUT_FLOW),
                 input_flow_str.c_str(), input_flow_str.size());

    string output_data_str = Format("%ld", it.second.output_data);
    oak_dict_add(&event->args,
                 OAK_M_OUTPUT_DATA, sizeof(OAK_M_OUTPUT_DATA),
                 output_data_str.c_str(), output_data_str.size());

    channel->get()->Push(event);
  }
  metrics->clear();
}

using AlarmMessage = std::pair<int64_t, string>;

void ReportAlarm(unique_ptr<Channel>* channel, const AlarmMessage& msg) {
  struct outgoing_event* event = new struct outgoing_event;
  event->type = ET_ALARM;
  oak_dict_init(&event->args);

  string task_id_str = Format("%ld", msg.first);
  oak_dict_add(&event->args,
               OAK_TASK_ID, sizeof(OAK_TASK_ID),
               task_id_str.c_str(), task_id_str.size());

  oak_dict_add(&event->args,
               OAK_M_INPUT_FLOW, sizeof(OAK_M_INPUT_FLOW),
               msg.second.c_str(), msg.second.size());

  channel->get()->Push(event);
}

int Parse(ParserHandleDict* handles, struct oak_metadata* metadata,
          struct oak_tag* tag) {
  auto it = handles->find(tag->protocol_type);
  if (it == handles->cend())
    return -1;
  return it->second.get()->Parse(
      &metadata->up_stream, &metadata->down_stream,
      &metadata->fields, &tag->fields);
}

void ParserThreadRoutine(ParserContext* context) {
  assert(context->state.load(std::memory_order_acquire) == THREAD_INIT);

  const string title =
      Format("Parser thread %d (TID %d)", context->id, System::GetThreadId());

  // Initialize thread environment.
  int stream_channel_size = context->config->stream_channel_size;
  if (stream_channel_size <= 0)
    stream_channel_size = 512;
  context->stream.reset(new Channel(stream_channel_size));

  int report_channel_size = context->config->report_channel_size;
  if (report_channel_size <= 0)
    report_channel_size = 1024;
  context->report.reset(new Channel(report_channel_size));

  for (auto const& it : context->config->modules) {
    if (!it.second.enable)
      continue;

    unique_ptr<ParserHandle> handle;
    if (ParserHandleFactory(it.second, &handle) != 0) {
      // report open module failed and disabling it.
      int64_t task_id = -1;
      auto pos = it.second.config.find(OAK_TASK_ID);
      if (pos != it.second.config.cend())
        task_id = std::stoll(pos->second);
      assert(task_id != -1);
      if (context->id == 0) {
        string logs = Format("Load code (%ld) failed", it.second.id);
        ReportAlarm(&context->report, std::make_pair(task_id, logs));
        OAK_INFO("%s: %s\n", title.c_str(), logs.c_str());
      }
      continue;
    }
    context->handles[it.second.id].reset(handle.release());
  }

  if (context->handles.empty())
    OAK_WARNING("%s: No one module has been enabled\n", title.c_str());

  struct timeval tv = {0, 0};
  gettimeofday(&tv, nullptr);
  uint64_t current_timepoint = tv.tv_sec;
  uint64_t next_timepoint = current_timepoint + context->report_interval;
  ParserMetricsDict metrics;

  // Setup thread state.
  OAK_INFO("%s: Running on the core %s\n",
           title.c_str(), HumanReadableCoreInfo(context->cores).c_str());
  context->state.store(THREAD_RUNNING, std::memory_order_release);

  // Entry thread loop
  int error_count = 0;
  struct oak_metadata* metadata;

  while (context->state.load(std::memory_order_relaxed) != THREAD_STOP) {
    if (context->handles.empty() || !context->stream->Pop(&metadata)) {
      if (error_count < 4)
        ++error_count;
      sleep(error_count);
      continue;
    }

    assert(metadata != nullptr);

    // locking to ensure that parser handle can not be update and remove.
    bool succeeded_once = false;
    for (int i = 0; i < metadata->num_tag; ++i) {
      struct oak_tag* tag = &(metadata->tag[i]);
      std::lock_guard<mutex> guard(context->lock);
      if (Parse(&context->handles, metadata, tag) == 0) {
        succeeded_once = true;
        if (tag->task_id >= 0) {
          if (metrics[tag->protocol_type].task_id == -1)
            metrics[tag->protocol_type].task_id = tag->task_id;
          assert(metrics[tag->protocol_type].task_id == tag->task_id);
          metrics[tag->protocol_type].input_flow += metadata->up_stream.size;
          metrics[tag->protocol_type].input_flow += metadata->down_stream.size;
          if (tag->fields.size > 0)
            metrics[tag->protocol_type].output_data += 1;
        }
      } else {
        oak_dict_clear(&(metadata->tag[i].fields));
      }
    }

    if (succeeded_once) {
      for (; context->offset < context->num_sink; ++context->offset) {
        auto sink = context->sink_ref[context->offset];

        // Skipping the offlined sink.
        if (sink->state.load(std::memory_order_acquire) != THREAD_RUNNING) {
          OAK_WARNING("Sink thread %d has offlined\n", sink->id);
          continue;
        }

        if (!sink->stream->Push(metadata)) {
          OAK_WARNING("Sink thread %d stream channel is fulling\n", sink->id);
          continue;
        }

        // Push to sink sucessful, release metadata ownership.
        metadata = nullptr;
        break;
      }

      if (context->offset >= context->num_sink)
        context->offset = 0;
    } else {
      unique_ptr<struct oak_metadata> free(metadata);
      oak_metadata_free(free.get());
    }

    tv = {0, 0};
    gettimeofday(&tv, nullptr);
    current_timepoint = tv.tv_sec;
    if (current_timepoint > next_timepoint) {
      ReportMetrics(&context->report, &metrics);
      next_timepoint = current_timepoint + context->report_interval;
    }
  }  // while ()

  // free context
  for (auto& it : context->handles)
    it.second.reset();

  while (context->stream->Pop(&metadata)) {
    unique_ptr<struct oak_metadata> free(metadata);
    oak_metadata_free(free.get());
  }

  struct outgoing_event* event = nullptr;
  while (context->stream->Pop(&event)) {
    unique_ptr<struct outgoing_event> free(event);
    oak_dict_free(&(free.get()->args));
  }
}

void SinkThreadRoutine(SinkContext* context) {
  assert(context->state.load(std::memory_order_acquire) == THREAD_INIT);

  const string title =
      Format("Sink thread %d (TID %d)", context->id, System::GetThreadId());

  // Initialize thread environment.
  int stream_channel_size = context->config->stream_channel_size;
  if (stream_channel_size <= 0)
    stream_channel_size = 512;
  context->stream.reset(new Channel(stream_channel_size));

  for (auto const& it : context->config->modules) {
    if (!it.second.enable)
      continue;

    unique_ptr<SinkHandle> handle;
    if (SinkHandleFactory(it.second, &handle) != 0) {
      ThrowStdRuntimeError(
          Format("%s: Open module %s failed.",
              title.c_str(), it.first.c_str()));
      continue;
    }
    context->handles.emplace_back(handle.release());
  }

  if (context->handles.empty())
    OAK_WARNING("%s: No one module enabled\n", title.c_str());

  // Setup thread state.
  OAK_INFO("%s: Running on the core %s\n",
           title.c_str(), HumanReadableCoreInfo(context->cores).c_str());
  context->state.store(THREAD_RUNNING, std::memory_order_release);

  // Entry thread loop
  int error_count = 0;
  unique_ptr<struct oak_metadata> metadata;
  while (context->state.load(std::memory_order_relaxed) != THREAD_STOP) {
    struct oak_metadata* ptr = nullptr;
    if (context->handles.empty() || !context->stream->Pop(&ptr)) {
      if (error_count < 4)
        ++error_count;
      sleep(error_count);
      continue;
    }
    error_count = 0;

    // owned the ptr
    assert(ptr != nullptr);
    metadata.reset(ptr);

    for (size_t i = 0; i < context->handles.size(); ++i) {
      // Skipping the offlined parser.
      if (context->handles[i]->Write(metadata.get()) != 0) {
        OAK_WARNING("%s: Module %s write failed\n",
                    title.c_str(), context->handles[i]->name().c_str());
      }
    }

    oak_metadata_free(metadata.get());
    metadata.reset();
  }  // while ()

  // free context
  for (auto& it : context->handles)
    it.reset();
  struct oak_metadata* ptr = nullptr;
  while (context->stream->Pop(&ptr)) {
    metadata.reset(ptr);
    oak_metadata_free(metadata.get());
  }
}

void InitSourceContext(int id, SourceContext* context, const SourceConfig* config) {
  context->id = id;
  context->state.store(THREAD_STOP);
  context->self = 0;
  context->config = config;
  for (int i = 0; i < OAK_THREAD_MAX; ++i)
    context->parser_ref[i] = nullptr;
  context->num_parser = 0;
  context->offset = 0;
}

void InitParserontext(int id, ParserContext* context, const ParserConfig* config) {
  context->id = id;
  context->state.store(THREAD_STOP);
  context->self = 0;
  context->config = config;
  for (int i = 0; i < OAK_THREAD_MAX; ++i)
    context->sink_ref[i] = nullptr;
  context->num_sink = 0;
  context->offset = 0;
}

void InitSinkContext(int id, SinkContext* context, const SinkConfig* config) {
  context->id = id;
  context->state.store(THREAD_STOP);
  context->self = 0;
  context->config = config;
}

}  // anonymous namespace

void InitRuntimeEnviron(RuntimeEnviron* env, const Config& config) {
  env->num_source_context = config.source.num_threads;
  if (env->num_source_context <= 0)
    env->num_source_context = 1;
  if (env->num_source_context > OAK_THREAD_MAX)
    env->num_source_context = OAK_THREAD_MAX;

  env->num_parser_context = config.parser.num_threads;
  if (env->num_parser_context <= 0)
    env->num_parser_context = 1;
  if (env->num_parser_context > OAK_THREAD_MAX)
    env->num_parser_context = OAK_THREAD_MAX;

  env->num_sink_context = config.sink.num_threads;
  if (env->num_sink_context <= 0)
    env->num_sink_context = 1;
  if (env->num_sink_context > OAK_THREAD_MAX)
    env->num_sink_context = OAK_THREAD_MAX;

  for (int i = 0; i < OAK_THREAD_MAX; ++i) {
    InitSourceContext(i, &(env->source_context[i]), &config.source);
    for (int j = 0; j < OAK_THREAD_MAX; ++j) {
      env->source_context[i].parser_ref[j] = &(env->parser_context[j]);
      env->source_context[i].num_parser = env->num_parser_context;
    }

    InitParserontext(i, &(env->parser_context[i]), &config.parser);
    for (int j = 0; j < OAK_THREAD_MAX; ++j) {
      env->parser_context[i].sink_ref[j] = &(env->sink_context[j]);
      env->parser_context[i].num_sink = env->num_sink_context;
    }

    InitSinkContext(i, &(env->sink_context[i]), &config.sink);
  }
}

// TODO(YUYUE): design an adaptive CPU allocation scheme
void CreateWorker(RuntimeEnviron* env) {
  // Create sink worker
  for (int i = 0; i < env->num_sink_context; ++i) {
    const LogicCore* core = System::GetNextAvailLogicCore();
    SinkContext* context = &(env->sink_context[i]);
    if (core == nullptr)
      ThrowStdOutOfRange("No one CPU cal be allocated.");
    context->cores.push_back(core);
    context->state.store(THREAD_INIT, std::memory_order_release);
    string name = Format("Sink-%d", context->id);
    context->self = System::CreateThread(
        name, &core->mask,
        std::bind(SinkThreadRoutine, context));
    while (context->state.load(std::memory_order_acquire) != THREAD_RUNNING) {
      struct timespec ts = {0, 20 * 1000 * 1000};
      nanosleep(&ts, nullptr);
    }
  }

  // Create parser worker
  for (int i = 0; i < env->num_parser_context; ++i) {
    const LogicCore* core = System::GetNextAvailLogicCore();
    ParserContext* context = &(env->parser_context[i]);
    if (core == nullptr)
      ThrowStdOutOfRange("No one CPU cal be allocated.");
    context->cores.push_back(core);
    context->state.store(THREAD_INIT, std::memory_order_release);
    string name = Format("Parser-%d", context->id);
    context->self = System::CreateThread(
        name, &core->mask,
        std::bind(ParserThreadRoutine, context));
    while (context->state.load(std::memory_order_acquire) != THREAD_RUNNING) {
      struct timespec ts = {0, 20 * 1000 * 1000};
      nanosleep(&ts, nullptr);
    }
  }

  // Create source worker
  for (int i = 0; i < env->num_source_context; ++i) {
    const LogicCore* core = System::GetNextAvailLogicCore();
    SourceContext* context = &(env->source_context[i]);
    if (core == nullptr)
      ThrowStdOutOfRange("No one CPU cal be allocated.");
    context->cores.push_back(core);
    context->state.store(THREAD_INIT, std::memory_order_release);
    string name = Format("Source-%d", context->id);
    context->self = System::CreateThread(
        name, &core->mask,
        std::bind(SourceThreadRoutine, context));
    while (context->state.load(std::memory_order_acquire) != THREAD_RUNNING) {
      struct timespec ts = {0, 20 * 1000 * 1000};
      nanosleep(&ts, nullptr);
    }
  }
}

int UpdateParser(ParserContext* context, const ModuleConfig& module_config) {
  unique_ptr<ParserHandle> handle;
  if (ParserHandleFactory(module_config, &handle) != 0) {
    // report open module failed and disabling it.
    int64_t task_id = -1;
    auto pos = module_config.config.find(OAK_TASK_ID);
    if (pos != module_config.config.cend())
      task_id = std::stoll(pos->second);
    assert(task_id != -1);
    if (context->id == 0) {
      string logs = Format("Load code (%ld) failed", module_config.id);
      ReportAlarm(&context->report, std::make_pair(task_id, logs));
      OAK_INFO("%s\n", logs.c_str());
    }
    return -1;
  }
  context->handles[module_config.id].reset(handle.release());
  return 0;
}

}  // namespace oak
