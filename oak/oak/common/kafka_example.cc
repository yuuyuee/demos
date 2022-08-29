// Copyright 2022 The Oak Authors.

#include <iostream>
#include <string>
#include "oak/common/kafka.h"

namespace {

void Producer(const std::string& bootstrap, const std::string& topic) {
  struct oak_dict config;
  oak_dict_init(&config);
  oak_dict_add(&config,
               "bootstrap.servers", sizeof("bootstrap.servers"),
               bootstrap.c_str(), bootstrap.size());
  oak::KafkaProducer producer(config);
  oak_dict_free(&config);

  std::string line;
  while (std::getline(std::cin, line)) {
    producer.Pruduce(topic, nullptr, &line);
  }
}

void Handler(const oak::StringPiece key, const oak::StringPiece value) {
  std::cout << "Key: " << key.data() << " Value: " << value.data() << std::endl;
}

void Consumer(const std::string& bootstrap, const std::string& topic) {
  struct oak_dict config;
  oak_dict_init(&config);
  oak_dict_add(&config,
              "bootstrap.servers", sizeof("bootstrap.servers"),
              bootstrap.c_str(), bootstrap.size());
  oak::KafkaConsumer consumer(config);
  oak_dict_free(&config);

  std::vector<std::string> topics;
  topics.push_back(topic);
  consumer.Subscribe(topics);

  while (true) {
    consumer.Consume(Handler, 5000);
  }
}


}  // anonymous namespace

int main(int argc, char* argv[]) {
  if (argc != 4) {
    std::cout << "Usage: %s [-p|-c] bootstrap topic" << std::endl;
    return -1;
  }

  std::string flags = argv[1];
  std::string bootstrap = argv[2];
  std::string topic = argv[3];

  if (flags == "-p") {
    Producer(bootstrap, topic);
  } else if (flags == "-c") {
    Consumer(bootstrap, topic);
  } else {
    std::cout << "Usage: %s [-p|-c] bootstrap topic" << std::endl;
  }

  return 0;
}