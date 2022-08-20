// Copyright 2022 The Oak Authors.

#include <oak/common/queue.h>

#include <atomic>
#include <chrono>   // NOLINT
#include <memory>
#include <thread>   // NOLINT
#include <vector>
#include <string>

#include "gtest/gtest.h"

namespace {

struct DestructChecker {
  static unsigned int numInstances;
  DestructChecker() { ++numInstances; }
  DestructChecker(const DestructChecker&) { ++numInstances; }
  DestructChecker& operator=(const DestructChecker&) { return *this; }
  ~DestructChecker() { --numInstances; }
};

unsigned int DestructChecker::numInstances = 0;

}  // anonymous namespace

TEST(QueueTest, QueueCorrectness) {

}

TEST(QueueTest, PerfTest) {

}

TEST(QueueTest, Destructor) {
  // Test that orphaned elements in a queue are destroyed.
  {
    oak::Queue<DestructChecker> queue(1024);
    for (int i = 0; i < 10; ++i) {
      EXPECT_TRUE(queue.Push(DestructChecker()));
    }

    EXPECT_EQ(DestructChecker::numInstances, 10U);

    {
      DestructChecker ignore;
      EXPECT_TRUE(queue.Pop(&ignore));
      EXPECT_TRUE(queue.Pop(&ignore));
    }

    EXPECT_EQ(DestructChecker::numInstances, 8U);
  }

  EXPECT_EQ(DestructChecker::numInstances, 0U);

  // Test the same thing in the case that the queue write pointer has
  // wrapped, but the Pop one hasn't.
  {
    oak::Queue<DestructChecker> queue(4);
    for (int i = 0; i < 3; ++i) {
      EXPECT_TRUE(queue.Push(DestructChecker()));
    }
    EXPECT_EQ(DestructChecker::numInstances, 3U);
    {
      DestructChecker ignore;
      EXPECT_TRUE(queue.Pop(&ignore));
    }
    EXPECT_EQ(DestructChecker::numInstances, 2U);
    EXPECT_TRUE(queue.Push(DestructChecker()));
    EXPECT_EQ(DestructChecker::numInstances, 3U);
  }
  EXPECT_EQ(DestructChecker::numInstances, 0U);
}

TEST(QueueTest, Capacity) {
  oak::Queue<int> queue(3);
  EXPECT_EQ(queue.Capacity(), 3U);
}
