// Copyright 2022 The Oak Authors.

#ifndef OAK_EVENT_EVENT_H_
#define OAK_EVENT_EVENT_H_

#include <vector>

namespace oak {

class Message {
 public:
  Message();
  virtual ~Message();

  virtual bool Serialize(std::vector<char>* buffer) = 0;
  virtual bool Deserialize(const std::vector<char>& buffer) = 0;
};

class Event {};

}  // namespace oak

#endif  // OAK_EVENT_EVENT_H_
