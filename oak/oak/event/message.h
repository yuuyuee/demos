// Copyright 2022 The Oak Authors.

#ifndef OAK_EVENT_MESSAGE_H_
#define OAK_EVENT_MESSAGE_H_

#include <string>

namespace oak {

#define OAK_TOPIC_TASK    "NFAP_CONFIG_TASK_PR"
#define OAK_TOPIC_METRICS "NFAP_REPORT_METRICS_PR"
#define OAK_TOPIC_ALARM   "NFAP_REPORT_ALARM_PR"

struct MessageHeader {
  static const int kDefaultVersion = 1;

  enum Command {
    ADD_TASK = 1,
  };

  std::string sender;
  std::string target;
  std::string send_time;
  int command;
  std::string version;
};

struct MessageBody {
  std::string task_id;
  bool is_extract_netflow;
};

struct Message {
  std::string md_id;
  MessageHeader header;
  MessageBody body;
};

}  // namespace oak

#endif  // OAK_EVENT_MESSAGE_H_
