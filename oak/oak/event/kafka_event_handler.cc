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

enum TaskType {
  STAND_PROTO_PARSE = 0,
  CUSTOM_PROTO_PARSE,
  DATA_CONTROL
};

#ifndef OAK_BUFFER_MAX
# define OAK_BUFFER_MAX 255
#endif

#ifndef OAK_TASK_MAX
# define OAK_TASK_MAX 32
#endif

struct AddTaskBody {
  // request
  struct {
    int task_id;          ///< task ID
    int task_type;        ///< task type
    int parent_task_id;   ///< parent task ID

    struct {                                  ///< customized code attr
      int decode_id;                          ///< code ID
      char file_name[OAK_BUFFER_MAX];         ///< file name
      char decode_ptype_en[OAK_BUFFER_MAX];   ///< protocol name
      int decode_ptype;                       ///< protocol value
      char decode_url[OAK_BUFFER_MAX];        ///< file URL
    } decode;

    union {
      struct {                    ///< customized code args
        int is_assemble;          ///< assemble
        int is_keep_meta;         ///< saved origin metadata
        int m_input_flow;         ///< count the input netflow
        int m_output_data;        ///< count the output records
      } custom_proto_parse_args;

      struct {                    ///< data control args
        int is_assemble;          ///< assemble
        int is_extract_netflow;   ///< record the communication
        int m_keep_flow;          ///< size of the saved netflow
      } data_control_args;
    };
  } request;

  // response
  struct {} response;
};

struct UpdateTaskBody {
  // request
  struct {
    int task_id;

    struct {
      int decode_id;
      char file_name[OAK_BUFFER_MAX];
      char decode_url[OAK_BUFFER_MAX];
    } decode;
  } request;

  // response
  struct {} response;
};

struct StopTaskBody {
  // request
  struct {
    int task_id[OAK_TASK_MAX];
  } request;

  struct {} response;
};

struct ClearAllTaskBody {
  // request
  struct {} request;
  // response
  struct {} response;
};

union Body {
  AddTaskBody add_task[OAK_TASK_MAX];   ///< ADD_TASK body
  int add_task_size;                    ///< number of ADD_TASK body
  UpdateTaskBody update_task;           ///< UPDATE_TASK body
  StopTaskBody stop_task;               ///< STOP_TASK body
  ClearAllTaskBody clear_all_task;      ///< CLEAR_ALL_TASK body
};

struct Header {
  std::string sender;                   ///< sender
  std::string target;                   ///< target
  uint64_t send_time;                   ///< send time in milli-seconds
  std::string command;                  ///< command
  int version;                          ///< message version, default 1
};

struct TaskRequest {
  std::string md_id;                    ///< reqeust message key
  Header header;                        ///< message header
  Body body;                            ///< message body
};

struct TaskResponse {
  std::string md_id;                    ///< message key
  Header header;                        ///< message header
  Body body;                            ///< message body
};

struct Alarm {
  std::string md_id;                    ///< message key
  Header header;                        ///< message header
  struct {
    uint64_t timestamp;                 ///< report timestamp
    std::string module;                 ///< report module name
    std::string subject;                ///< report subject
    int alarm_type;                     ///< alarm type
    int alarm_level;                    ///< alarm level, range 1 - 4
                                        ///< default 4(least level)
    void* items;                        ///< report details
  } body;
};

struct AlarmEnd {
  std::string md_id;                    ///< message key
  Header header;                        ///< message header
  struct {
    uint64_t timestamp;                 ///< report timestamp
    std::string module;                 ///< report module name
    std::string subject;                ///< report subject
  } body;
};

struct Metrics {
  std::string md_id;                    ///< message key
  Header header;                        ///< message header
  struct {
    uint64_t timestamp;                 ///< report timestamp
    std::string module;                 ///< report module name
    std::string subject;                ///< report subject
    void* items;                        ///< report details
  } body;
};


}  // anonymous namespace
}  // namespace oak

extern "C" {
static void* kafka_event_init(const struct oak_dict* config) {
  // TODO(YUYUE):
  return 0;
}

static int kafka_event_recv(void* context,
                            struct incoming_event* event) {
  // TODO(YUYUE):
  return -1;
}

static int kafka_event_pull(void* context,
                            struct incoming_event* event,
                            int* size) {
  // TODO(YUYUE):
  return -1;
}

static int kafka_event_send(void* context,
                            const struct outgoing_event* event) {
  // TODO(YUYUE):
  return -1;
}

static void kafka_event_close(void* context) {
  // TODO(YUYUE):
}

const struct oak_event_module kafka_event_handler = {
  .version = OAK_VERSION,
  .flags = OAK_MODULE_EVENT,
  .init = kafka_event_init,
  .pull = kafka_event_pull,
  .recv = kafka_event_recv,
  .send = kafka_event_send,
  .close = kafka_event_close
};

}  // extern "C"
