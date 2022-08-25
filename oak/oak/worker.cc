// Copyright 2022 The Oak Authors.

#include "oak/worker.h"

#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <string>

#include "oak/common/format.h"
#include "oak/common/throw_delegate.h"
#include "oak/logging/logging.h"
#include "oak/addons/metadata_internal.h"
#include "oak/addons/public/event_define.h"

using std::unique_ptr;
using std::vector;
using std::unordered_map;
using std::atomic;
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

}  // anonymous namespace

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

    // No one parser has living, free metadata and copy
    if (copy) {
      oak_metadata_free(copy);
      delete copy;
    }
  }  // while ()

  context->handle.reset();
}

namespace {

struct Metrics {
  uint64_t task_id;   // task ID

  size_t input_flow{0};
  size_t output_data{0};
  size_t keep_flow{0};

  static int Factory(const Dict& module_config, Metrics* metrics);
};


}  // anonymous namespace

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
  context->stream.reset(new Channel(stream_channel_size));

  for (auto const& it : context->config->modules) {
    if (!it.second.enable)
      continue;

    unique_ptr<ParserHandle> handle;
    if (ParserHandleFactory(it.second, &handle) != 0) {
      // TODO(YUYUE): report open module failed
      continue;
    }
    context->handles[it.second.id].reset(handle.release());
  }

  if (context->handles.empty())
    OAK_WARNING("%s: No one module enabled\n", title.c_str());

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

    // owned the ptr
    assert(metadata != nullptr);
    struct oak_dict fields;
    oak_dict_init(&fields);

    // locking to ensure that parser handle can not be update and remove.
    context->lock.lock();
    auto pos = context->handles.find(metadata->parser_id);
    if (pos == context->handles.cend()) {
      OAK_WARNING("%s: protocol %ld not found anyone parser\n",
                  title.c_str(), metadata->parser_id);
      unique_ptr<struct oak_metadata> free(metadata);
      oak_metadata_free(free.get());
      context->lock.unlock();
      continue;
    }

    ParserHandle* handle = pos->second.get();
    int parse_ret = handle->Parse(&metadata->up_stream,
                                  &metadata->down_stream,
                                  &metadata->fields, &fields);
    int mark_ret = handle->Mark(&metadata->up_stream,
                                &metadata->down_stream);
    context->lock.unlock();

    if (parse_ret == 0 && fields.size > 0) {
      oak_dict_free(&metadata->fields);
      metadata->fields = fields;
    } else {
      oak_dict_clear(&metadata->fields);
      oak_dict_free(&fields);
    }

    if (mark_ret == 0) {
      //
    } else {
      oak_dict_clear(&metadata->communication);
    }

    if (parse_ret != 0 && mark_ret != 0) {

    }
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



void CreateWorker(const Config& config) {
  // for (int i = 0; i < config.source.num_threads; ++i) {
  //   LogicCore* logic_core = System::GetNextAvailableLogicCore();
  //   if (logic_core == nullptr)
  //     ThrowStdOutOfRange("No enough available CPU");
  //   oak::System::CreateThread(oak::Format("source-%d", i),
  //                             logic_core->mask,
  //                             DummyFun);
  // }

  // for (int i = 0; i < config.parser.num_threads; ++i) {
  //   LogicCore* logic_core = System::GetNextAvailableLogicCore();
  //   if (logic_core == nullptr)
  //     ThrowStdOutOfRange("No enough available CPU");
  //   oak::System::CreateThread(oak::Format("parser-%d", i),
  //                             logic_core->mask,
  //                             DummyFun);
  // }

  // for (int i = 0; i < config.sink.num_threads; ++i) {
  //   LogicCore* logic_core = System::GetNextAvailableLogicCore();
  //   if (logic_core == nullptr)
  //     ThrowStdOutOfRange("No enough available CPU");
  //   oak::System::CreateThread(oak::Format("sink-%d", i),
  //                             logic_core->mask,
  //                             DummyFun);
  // }
}

}  // namespace oak
