// Copyright 2022 The Oak Authors.

#ifndef OAK_COMMON_QUEUE_H_
#define OAK_COMMON_QUEUE_H_

#include <stdlib.h>
#include <assert.h>

#include <atomic>
#include <memory>
#include <type_traits>
#include <utility>

#include "oak/common/macros.h"

namespace oak {

// Queue
// This class is a one producer and one consumer queue without locks.
template <typename Tp>
class Queue {
 public:
  using value_type = Tp;

  // Construct object with a fixed maximum size.
  explicit Queue(uint32_t size) noexcept
      : size_(size < 2 ? 2 : size),
        records_(static_cast<Tp*>(malloc(sizeof(Tp) * size))),
        read_index_(0),
        write_index_(0) {}

  // Destruct anything that may still exist in the queue.
  // Note: only one thread can be doing this.
  ~Queue() {
    if (!std::is_trivially_destructible<Tp>::value) {
      size_t read_index = read_index_;
      size_t end_index = write_index_;
      while (read_index != end_index) {
        records_[read_index].~Tp();
        if (++read_index == size_) {
          read_index = 0;
        }
      }
    }

    free(records_);
  }

  template <typename... Args>
  bool Write(Args&&... args) {
    auto const current_write = write_index_.load(std::memory_order_relaxed);
    auto next_record = current_write + 1;
    if (next_record == size_)
      next_record = 0;
    if (next_record != read_index_.load(std::memory_order_acquire)) {
      new (&records_[current_write]) Tp(std::forward<Args>(args)...);
      write_index_.store(next_record, std::memory_order_release);
      return true;
    }
    return false;
  }

  // Move (or copy) the value at the front of the queue to given variable
  bool Read(Tp* record) {
    auto const current_read = read_index_.load(std::memory_order_relaxed);
    if (current_read == write_index_.load(std::memory_order_acquire))
      return false;

    auto next_record = current_read + 1;
    if (next_record == size_) {
      next_record = 0;
    }
    *record = std::move(records_[current_read]);
    records_[current_read].~Tp();
    read_index_.store(next_record, std::memory_order_release);
    return true;
  }

  // Pointer to the value at the front of the queue (for use in-place) or
  // nullptr if empty.
  Tp* Front() {
    auto const current_read = read_index_.load(std::memory_order_relaxed);
    if (current_read == write_index_.load(std::memory_order_acquire))
      return nullptr;
    return &records_[current_read];
  }

  // Queue must not be empty
  void PopFront() {
    auto const current_read = read_index_.load(std::memory_order_relaxed);
    assert(current_read != write_index_.load(std::memory_order_acquire));

    auto next_record = current_read + 1;
    if (next_record == size_)
      next_record = 0;
    records_[current_read].~Tp();
    read_index_.store(next_record, std::memory_order_release);
  }

  bool Empty() const {
    return read_index_.load(std::memory_order_acquire) ==
        write_index_.load(std::memory_order_acquire);
  }

  bool Full() const {
    auto next_record = write_index_.load(std::memory_order_acquire) + 1;
    if (next_record == size_)
      next_record = 0;
    if (next_record != read_index_.load(std::memory_order_acquire))
      return false;
    return true;
  }

  // If called by consumer, then true size may be more because producer may
  // be adding items concurrently.
  // If called by producer, then true size may be less because consumer may
  // be removing items concurrently.
  // It is undefined to call this from any other thread.
  size_t SizeGuess() const {
    int ret = write_index_.load(std::memory_order_acquire) -
        read_index_.load(std::memory_order_acquire);
    if (ret < 0)
      ret += size_;
    return ret;
  }

  // Maximum number of items in the queue.
  size_t Capacity() const { return size_ - 1; }

 private:
  Queue(Queue const&) = delete;
  Queue& operator=(const Queue&) = delete;

  char padding0_[OAK_CACHELINE_SIZE];
  const uint32_t size_;
  Tp* const records_;
  char padding1_[OAK_CACHELINE_SIZE - sizeof(uint32_t) - sizeof(Tp*)];

  std::atomic<unsigned int> read_index_;
  char padding2_[OAK_CACHELINE_SIZE - sizeof(std::atomic<unsigned int>)];

  std::atomic<unsigned int> write_index_;
  char padding3_[OAK_CACHELINE_SIZE - sizeof(std::atomic<unsigned int>)];
};

}  // namespace oak

#endif  // OAK_COMMON_QUEUE_H_

