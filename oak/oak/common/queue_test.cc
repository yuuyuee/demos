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

template <class Tp>
struct TestTraits {
  Tp Limit() const { return 1 << 24; }
  Tp Generate() const { return rand() % 26; }
};

template <>
struct TestTraits<std::string> {
  unsigned int Limit() const { return 1 << 22; }
  std::string Generate() const { return std::string(12, ' '); }
};

template <class QueueType, size_t Size, bool Pop = false>
struct PerfTest {
  using Tp = typename QueueType::value_type;

  PerfTest() : queue_(Size), done_(false) {}

  void operator()() {
    auto const start_time = std::chrono::system_clock::now();

    std::thread producer([this] { this->Producer(); });
    std::thread consumer([this] { this->Consumer(); });

    producer.join();
    done_ = true;
    consumer.join();

    auto duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now() - start_time);
  }

  void Producer() {
    for (auto i = traits_.Limit(); i > 0; --i) {
      while (!queue_.Write(traits_.Generate())) {
        // Loop
      }
    }
  }

  void Consumer() {
    if (Pop) {
      while (!done_) {
        if (queue_.Front()) {
          queue_.PopFront();
        }
      }
    } else {
      while (!done_) {
        Tp data;
        queue_.Read(&data);
      }
    }
  }

  QueueType queue_;
  std::atomic<bool> done_;
  TestTraits<Tp> traits_;
};

template <class TestType>
void doTest(const char*) {
  std::unique_ptr<TestType> const t(new TestType());
  (*t)();
}

template <class Tp, bool Pop = false>
void PerfTestType(const char*) {
  const size_t size = 0xfffe;
  doTest<PerfTest<oak::Queue<Tp>, size, Pop>>("Queue");
}

template <class QueueType, size_t Size, bool Pop>
struct CorrectnessTest {
  using Tp = typename QueueType::value_type;

  CorrectnessTest() : queue_(Size), done_(false) {
    const size_t testSize = traits_.Limit();
    test_data_.reserve(testSize);
    for (size_t i = 0; i < testSize; ++i) {
      test_data_.push_back(traits_.Generate());
    }
  }

  void operator()() {
    std::thread producer([this] { this->Producer(); });
    std::thread consumer([this] { this->Consumer(); });

    producer.join();
    done_ = true;
    consumer.join();
  }

  void Producer() {
    for (auto& data : test_data_) {
      while (!queue_.Write(data)) {
      }
    }
  }

  void Consumer() {
    if (Pop) {
      ConsumerPop();
    } else {
      ConsumerRead();
    }
  }

  void ConsumerPop() {
    for (auto expect : test_data_) {
    again:
      Tp* data;
      if (!(data = queue_.Front())) {
        if (done_) {
          if (!(data = queue_.Front())) {
            EXPECT_TRUE(0 && "Finished too early");
            return;
          }
        } else {
          goto again;
        }
        EXPECT_EQ(*data, expect);
      } else {
        EXPECT_EQ(*data, expect);
        queue_.PopFront();
      }
    }
  }

  void ConsumerRead() {
    for (auto expect : test_data_) {
    again:
      Tp data;
      if (!queue_.Read(&data)) {
        if (done_) {
          if (!queue_.Read(&data)) {
            EXPECT_TRUE(0 && "Finished too early ...");
            return;
          }
        } else {
          goto again;
        }
      }
      EXPECT_EQ(data, expect);
    }
  }

  std::vector<Tp> test_data_;
  QueueType queue_;
  TestTraits<Tp> traits_;
  std::atomic<bool> done_;
};

template <class Tp, bool Pop = false>
void CorrectnessTestType(const std::string&) {
  doTest<CorrectnessTest<oak::Queue<Tp>, 0xfffe, Pop>>("Queue");
}

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
  CorrectnessTestType<std::string, true>("string (front+pop)");
  CorrectnessTestType<std::string>("string");
  CorrectnessTestType<int>("int");
  CorrectnessTestType<unsigned long long>("unsigned long long");  // NOLINT
}

TEST(QueueTest, PerfTest) {
  PerfTestType<std::string, true>("string (front+pop)");
  PerfTestType<std::string>("string");
  PerfTestType<int>("int");
  PerfTestType<unsigned long long>("unsigned long long");  // NOLINT
}

TEST(QueueTest, Destructor) {
  // Test that orphaned elements in a queue are destroyed.
  {
    oak::Queue<DestructChecker> queue(1024);
    for (int i = 0; i < 10; ++i) {
      EXPECT_TRUE(queue.Write(DestructChecker()));
    }

    EXPECT_EQ(DestructChecker::numInstances, 10U);

    {
      DestructChecker ignore;
      EXPECT_TRUE(queue.Read(&ignore));
      EXPECT_TRUE(queue.Read(&ignore));
    }

    EXPECT_EQ(DestructChecker::numInstances, 8U);
  }

  EXPECT_EQ(DestructChecker::numInstances, 0U);

  // Test the same thing in the case that the queue write pointer has
  // wrapped, but the Read one hasn't.
  {
    oak::Queue<DestructChecker> queue(4);
    for (int i = 0; i < 3; ++i) {
      EXPECT_TRUE(queue.Write(DestructChecker()));
    }
    EXPECT_EQ(DestructChecker::numInstances, 3U);
    {
      DestructChecker ignore;
      EXPECT_TRUE(queue.Read(&ignore));
    }
    EXPECT_EQ(DestructChecker::numInstances, 2U);
    EXPECT_TRUE(queue.Write(DestructChecker()));
    EXPECT_EQ(DestructChecker::numInstances, 3U);
  }
  EXPECT_EQ(DestructChecker::numInstances, 0U);
}

TEST(QueueTest, EmptyFull) {
  oak::Queue<int> queue(3);
  EXPECT_TRUE(queue.Empty());
  EXPECT_FALSE(queue.Full());

  EXPECT_TRUE(queue.Write(1));
  EXPECT_FALSE(queue.Empty());
  EXPECT_FALSE(queue.Full());

  EXPECT_TRUE(queue.Write(2));
  EXPECT_FALSE(queue.Empty());
  // Tricky: full after 2 writes, not 3.
  EXPECT_TRUE(queue.Full());

  EXPECT_FALSE(queue.Write(3));
  EXPECT_EQ(queue.SizeGuess(), 2U);
}

TEST(QueueTest, Capacity) {
  oak::Queue<int> queue(3);
  // QueueTest max size is buffer size - 1.
  EXPECT_EQ(queue.Capacity(), 2U);
}
