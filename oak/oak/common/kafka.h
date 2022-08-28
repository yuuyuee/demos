// Copyright 2022 The Oak Authors.

#ifndef OAK_COMMON_KAFKA_H_
#define OAK_COMMON_KAFKA_H_

#include <memory>
#include <string>
#include <functional>
#include <unordered_map>

namespace oak {

using Dict = std::unordered_map<std::string, std::string>;

class KafkaConsumer {
 public:
  using MessageHandler = std::function<void(const char*, size_t)>;

  explicit KafkaConsumer(const Dict& config);
  ~KafkaConsumer();

  void Consume(int timeout_ms);

 private:
  KafkaConsumer(KafkaConsumer const&) = delete;
  KafkaConsumer& operator=(KafkaConsumer const&) = delete;

  class Impl;
  std::unique_ptr<Impl> impl_;
};

class KafkaProducer {
 public:
  explicit KafkaProducer(const Dict& config);
  ~KafkaProducer();

  // buffer will free(3) when it is done.
  void Pruduce(const std::string& buffer);

 private:
  KafkaProducer(KafkaProducer const&) = delete;
  KafkaProducer& operator=(KafkaProducer const&) = delete;

  class Impl;
  std::unique_ptr<Impl> impl_;
};

}  // namespace oak

#endif  // OAK_COMMON_KAFKA_H_
