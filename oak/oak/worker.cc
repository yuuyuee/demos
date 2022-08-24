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

using std::unique_ptr;
using std::vector;
using std::unordered_map;
using std::atomic;
using std::string;
using oak::Channel;

namespace oak {

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
  assert(context->status.load(std::memory_order_acquire) == THREAD_INIT);

  // Initialize thread environment.
  for (auto const& it : context->args) {
    unique_ptr<SourceHandle> handle;
    int ret = SourceHandleFactory(it, &handle);
    if (ret != 0) {
      ThrowStdRuntimeError(
          Format("Open module %s failed.\n", it.module_name.c_str()));
      continue;
    }
    context->handles.emplace_back(handle.release());
  }

  // Setup thread status.
  OAK_INFO("Source thread %d (TID %d - %ld) is running on the core %s\n",
           context->id,
           System::GetThreadId(),
           context->self,
           HumanReadableCoreInfo(context->cores).c_str());
  context->status.store(THREAD_RUNNING, std::memory_order_release);

  // Entry thread loop
  size_t index = 0;
  int error_count = 0;
  unique_ptr<struct oak_metadata> metadata;
  while (context->status.load(std::memory_order_relaxed) != THREAD_STOP) {
    if (!metadata) {
      struct oak_metadata* ptr =
          reinterpret_cast<struct oak_metadata*>(malloc(sizeof(*ptr)));
      oak_metadata_init(ptr);
      metadata.reset(ptr);
    }

    if (context->handles[index]->Read(metadata.get()) != 0) {
      if (error_count < 5)
        ++error_count;
      sleep(error_count);
      continue;
    }
    error_count = 0;


    for (; index < context->num_parser; ++index) {
      auto parser = context->parser_ref[index];

      // Skipping the offlined parser.
      if (parser->status.load(std::memory_order_acquire) != THREAD_RUNNING)
        continue;

      if (parser->stream->Push(metadata.get())) {
        // Push to parser sucessful, release metadata ownership.
        metadata.release();
      }
    }

    if (index == context->num_parser)
      index = 0;

    // No one parser has living, clearing the metadata for next used.
    if (metadata)
      oak_metadata_clear(metadata.get());
  }  // while ()
}

void ParserThreadRoutine(ParserContext* context) {
  assert(context->status.load(std::memory_order_acquire) == THREAD_INIT);

  // Initialize thread environment.
  context->stream.reset(new Channel(2048));
  context->report.reset(new Channel(128));

  for (auto const& it : context->args) {
    unique_ptr<ParserHandle> handle;
    int ret = ParserHandleFactory(it.second, &handle);
    if (ret != 0) {
      // TODO(YUYUE): report
      continue;
    }
    context->handles[it.first].reset(handle.release());
  }

  // Setup thread status.
  OAK_INFO("Sink thread %d (TID %d - %ld) is running on the core %s\n",
           context->id,
           System::GetThreadId(),
           context->self,
           HumanReadableCoreInfo(context->cores).c_str());
  context->status.store(THREAD_RUNNING, std::memory_order_release);
}

void SinkThreadRoutine(SinkContext* context) {
  assert(context->status.load(std::memory_order_acquire) == THREAD_INIT);

  // Initialize thread environment.
  context->stream.reset(new Channel(2048));

  for (auto const& it : context->args) {
    unique_ptr<SinkHandle> handle;
    int ret = SinkHandleFactory(it, &handle);
    if (ret != 0) {
      ThrowStdRuntimeError(
          Format("Open module %s failed.\n", it.module_name.c_str()));
      continue;
    }
    context->handles.emplace_back(handle.release());
  }

  // Setup thread status.
  OAK_INFO("Sink thread %d (TID %d - %ld) is running on the core %s\n",
           context->id,
           System::GetThreadId(),
           context->self,
           HumanReadableCoreInfo(context->cores).c_str());
  context->status.store(THREAD_RUNNING, std::memory_order_release);

  // Entry thread loop
  int error_count = 0;
  unique_ptr<struct oak_metadata> metadata;
  while (context->status.load(std::memory_order_relaxed) != THREAD_STOP) {
    struct oak_metadata* ptr = nullptr;
    if (!context->stream->Pop(&ptr)) {
      if (error_count < 5)
        ++error_count;
      sleep(error_count);
      continue;
    }

    // owned the ptr
    assert(ptr != nullptr);
    metadata.reset(ptr);

    for (size_t i = 0; i < context->handles.size(); ++i) {
      // Skipping the offlined parser.
      if (context->handles[i]->Write(metadata.get()) != 0) {
        OAK_WARNING("Sink module %s: write failed\n",
                    context->handles[i]->name().c_str());
      }
    }
  }  // while ()
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
