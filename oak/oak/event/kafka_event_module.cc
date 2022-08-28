// Copyright 2022 The Oak Authors.

#include <stdint.h>
#include <string>

#include "oak/addons/public/event.h"

namespace oak {
namespace {
#define SELF_NAME   "PR"
#define TARGET_NAME "APP"

#define TOPIC_CONFIG      "NFAP_CONFIG_TASK_" SELF_NAME
#define TOPIC_CONFIG_ACK  "NFAP_ACKCFG_TASK_" SELF_NAME
#define TOPIC_METRICS     "NFAP_REPORT_METRICS_" SELF_NAME
#define TOPIC_REPORT      "NFAP_REPORT_ALARM_" SELF_NAME

#define MSG_DEFAULT_VERSION 1
#define MSG_MODULE_NAME     "CUSTOMIZE_RS"
#define MSG_GET_URL(page_no, page_size)     \
    "/api/v1/nfap/service/task/" SELF_NAME  \
    "?pageNo=" OAK_STRINGIFY(page_no)       \
    "&pageSize=" OAK_STRINGIFY(page_size)

#define CMD_ADD_TASK        "ADD_TASK"
#define CMD_UPDATE_TASK     "UPDATE_TASK"
#define CMD_STOP_TASK       "STOP_TASK"
#define CMD_CLEAR_ALL_TASK  "CLEAR_ALL_TASK"

#define CMD_ALARM     "ALARM_" MSG_MODULE_NAME
#define CMD_ALARM_END "ALARM_" MSG_MODULE_NAME "_END"
#define CMD_METRICS   "REPORT_" MSG_MODULE_NAME


int kafka_event_init(void** context, const struct oak_dict* config) {
  // TODO(YUYUE):
  printf("kafka_event_init\n");
  for (size_t i = 0; i < config->size; ++i) {
    printf("\t%s = %s\n",
        (const char*) config->elems[i].key.ptr,
        (const char*) config->elems[i].value.ptr);
  }
  return 0;
}

int kafka_event_recv(void* context, struct incoming_event* event) {
  // TODO(YUYUE):
  return -1;
}

int kafka_event_pull(void* context, struct incoming_event* event, int size) {
  printf("kafka_event_pull\n");
  // TODO(YUYUE):
  return 0;
}

int kafka_event_send(void* context, const struct outgoing_event* event) {
  // TODO(YUYUE):
  return -1;
}

void kafka_event_close(void* context) {
  // TODO(YUYUE):
}

extern "C" struct oak_event_module kafka_event_module;

struct oak_event_module kafka_event_module = {
  .version = OAK_VERSION,
  .flags = OAK_MODULE_EVENT,
  .init = oak::kafka_event_init,
  .pull = oak::kafka_event_pull,
  .recv = oak::kafka_event_recv,
  .send = oak::kafka_event_send,
  .close = oak::kafka_event_close
};

}  // anonymous namespace
}  // namespace oak
