// Copyright 2022 The Oak Authors.

#ifndef OAK_COMMON_CHANNEL_H_
#define OAK_COMMON_CHANNEL_H_

#include <stdlib.h>
#include <assert.h>

#include <atomic>
#include <utility>

namespace oak {

// Queue
// This class is a multi producer and multi consumer queue without locks.

class Channel {
 public:
  // Construct object with a fixed maximum size.
  explicit Channel(uint32_t size, void** ptr = nullptr) noexcept
      : ownship_(false), size_(size < 2 ? 2 : size), records_(ptr),
        cons_head_(0), cons_tail_(0),
        prod_head_(0), prod_tail_(0) {
    if (!records_) {
      records_ = static_cast<void**>(malloc(sizeof(void*) * size));
      assert(records_ && "No enough memory");
      ownship_ = true;
    }
  }

  // Destruct object.
  ~Channel() {
    if (ownship_)
      free(records_);
  }

  inline bool Push(void* ptr) {
    auto prod_head = prod_head_.load(std::memory_order_relaxed);
    auto next_prod_head = 0U;

    do {
      next_prod_head = prod_head + 1;
      if (next_prod_head == size_)
        next_prod_head = 0;
      std::atomic_thread_fence(std::memory_order_acquire);
      if (next_prod_head == cons_tail_.load(std::memory_order_acquire))
        return false;
    } while (!prod_head_.compare_exchange_weak(
             prod_head, next_prod_head,
             std::memory_order_relaxed,
             std::memory_order_relaxed));

    records_[prod_head] = ptr;

    while (prod_tail_.load(std::memory_order_relaxed) != prod_head) {
      // wait for completion of the other thread
      assert(prod_tail_ < size_);
    }
    prod_tail_.store(next_prod_head, std::memory_order_release);
    return true;
  }

  template <typename Tp>
  inline bool Pop(Tp** ptr) {
    auto cons_head = cons_head_.load(std::memory_order_relaxed);
    auto next_cons_head = 0U;

    do {
      std::atomic_thread_fence(std::memory_order_acquire);
      if (cons_head == prod_tail_.load(std::memory_order_acquire))
        return false;
      next_cons_head = cons_head + 1;
      if (next_cons_head == size_)
        next_cons_head = 0;
    } while (!cons_head_.compare_exchange_strong(
             cons_head, next_cons_head,
             std::memory_order_relaxed,
             std::memory_order_relaxed));

    *ptr = static_cast<Tp*>(records_[cons_head]);

    while (cons_tail_.load(std::memory_order_relaxed) != cons_head) {
      // wait for completion of the other thread
      assert(cons_tail_ < size_);
    }
    cons_tail_.store(next_cons_head, std::memory_order_release);
    return true;
  }

  // Maximum number of items in the queue.
  constexpr size_t Capacity() const { return size_; }

 private:
  Channel(Channel const&) = delete;
  Channel& operator=(const Channel&) = delete;

  char padding0_[64];
  bool ownship_;
  const uint32_t size_;
  void** records_;

  alignas(64) std::atomic<unsigned int> cons_head_;
  alignas(64) std::atomic<unsigned int> cons_tail_;
  alignas(64) std::atomic<unsigned int> prod_head_;
  alignas(64) std::atomic<unsigned int> prod_tail_;
};

}  // namespace oak

#endif  // OAK_COMMON_CHANNEL_H_

