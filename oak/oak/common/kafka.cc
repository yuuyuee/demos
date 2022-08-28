// Copyright 2022 The Oak Authors.

#include "oak/common/kafka.h"
#include <librdkafka/rdkafkacpp.h>

using std::string;

namespace oak {

// KafkaConsumer

class KafkaConsumer::Impl {
 public:
  explicit Impl(const Dict& config);
  ~Impl();

  void Consume(int timeout_ms);

 private:
  Impl(Impl const&) = delete;
  Impl& operator=(Impl const&) = delete;
};

KafkaConsumer::Impl::Impl(const Dict& config) {

}

KafkaConsumer::Impl::~Impl() {

}

void KafkaConsumer::Impl::Consume(int timeout_ms) {

}

KafkaConsumer::KafkaConsumer(const Dict& config)
    : impl_(new Impl(config)) {}
KafkaConsumer::~KafkaConsumer() {}

void KafkaConsumer::Consume(int timeout_ms) {
  return impl_->Consume(timeout_ms);
}

// KafkaProducer

class KafkaProducer::Impl {
 public:
  explicit Impl(const Dict& config);
  ~Impl();

  // buffer will free(3) when it is done.
  void Pruduce(const string& buffer);

 private:
  Impl(Impl const&) = delete;
  Impl& operator=(Impl const&) = delete;
};

KafkaProducer::Impl::Impl(const Dict& config) {

}

KafkaProducer::Impl::~Impl() {

}

void KafkaProducer::Impl::Pruduce(const string& buffer) {

}

KafkaProducer::KafkaProducer(const Dict& config)
    : impl_(new Impl(config)) {}
KafkaProducer::~KafkaProducer() {}

void KafkaProducer::Pruduce(const string& buffer) {
  return impl_->Pruduce(buffer);
}

}  // namespace oak
