// Copyright 2022 The Oak Authors.

#ifndef OAK_COMMON_QUEUE_H_
#define OAK_COMMON_QUEUE_H_

#include <stdlib.h>
#include <assert.h>

#include <atomic>
#include <memory>
#include <type_traits>
#include <utility>

namespace oak {

// Queue
// This class is a multi producer and one consumer queue without locks.
template <typename Tp>
class Queue {
 public:
  using value_type = Tp;

  // Construct object with a fixed maximum size.
  explicit Queue(uint32_t size) noexcept
      : size_(size < 2 ? 2 : size),
        records_(static_cast<Tp*>(malloc(sizeof(Tp) * size))),
        cons_head_(0),
        prod_head_(0),
        prod_tail_(0) {}

  // Destruct anything that may still exist in the queue.
  // Note: only one thread can be doing this.
  ~Queue() {
    if (!std::is_trivially_destructible<Tp>::value) {
      size_t cons_head = cons_head_;
      size_t prod_tail = prod_tail_;
      while (cons_head != prod_tail) {
        records_[cons_head].~Tp();
        if (++cons_head == size_)
          cons_head = 0;
      }
    }

    free(records_);
  }

  template <typename... Args>
  bool Push(Args&&... args) {
    auto prod_head = prod_head_.load(std::memory_order_relaxed);
    auto next_prod_head = 0U;

    do {
      next_prod_head = prod_head + 1;
      if (next_prod_head == size_)
        next_prod_head = 0;

      if (next_prod_head == cons_head_.load(std::memory_order_acquire))
        return false;
    } while (!prod_head_.compare_exchange_strong(
             prod_head, next_prod_head,
             std::memory_order_acq_rel,
             std::memory_order_relaxed));

    new(&records_[prod_head]) Tp(std::forward<Args>(args)...);

    while (!prod_tail_.compare_exchange_strong(
            prod_head, next_prod_head,
            std::memory_order_acq_rel,
            std::memory_order_relaxed)) {}
    return true;
  }

  // Move (or copy) the value at the front of the queue to given variable
  bool Pop(Tp* record) {
    auto cons_head = cons_head_.load(std::memory_order_relaxed);
    if (cons_head == prod_tail_.load(std::memory_order_acquire))
      return false;

    *record = std::move(records_[cons_head]);
    records_[cons_head].~Tp();

    cons_head += 1;
    if (cons_head == size_)
      cons_head = 0;

    cons_head_.store(cons_head, std::memory_order_release);
    return true;
  }

  // Maximum number of items in the queue.
  size_t Capacity() const { return size_; }

 private:
  Queue(Queue const&) = delete;
  Queue& operator=(const Queue&) = delete;

  char padding0_[64];
  const uint32_t size_;
  Tp* const records_;
  char padding1_[64 - sizeof(uint32_t) - sizeof(Tp*)];

  std::atomic<unsigned int> cons_head_;
  char padding2_[64 - sizeof(std::atomic<unsigned int>)];

  std::atomic<unsigned int> prod_head_;
  char padding3_[64 - sizeof(std::atomic<unsigned int>)];

  std::atomic<unsigned int> prod_tail_;
  char padding4_[64 - sizeof(std::atomic<unsigned int>)];
};

}  // namespace oak

#endif  // OAK_COMMON_QUEUE_H_

