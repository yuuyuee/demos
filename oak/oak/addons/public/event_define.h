/* Copyright 2022 The Oak Authors. */

#ifndef OAK_ADDONS_PUBLIC_EVENT_DEFINE_H_
#define OAK_ADDONS_PUBLIC_EVENT_DEFINE_H_

#include "oak/addons/public/dict.h"

#ifdef __cplusplus
extern "C" {
#endif

enum event_type {
  /* below for incomming event */
  ET_PARSER = 0,

  /* below for outgoing event */
  ET_ALARM = 1000,
  ET_METRICS
};

enum event_subtype {
  ET_CMD_ADD = 0,
  ET_UPDATE,
  ET_REMOVE,
  ET_CLEAR
};

/* below attributes for protocol parser arguments */

/* protocol type */
#define OAK_PROTOCOL_TYPE "protocol_type"

/* protocol name */
#define OAK_PROTOCOL_NAME "protocol_name"

/* parser file name */
#define OAK_FILE_NAME "file_name"

/* parser http URL for download */
#define OAK_HTTP_URL "http_url"

/* application type */
#define OAK_APP_TYPE "app_type"

struct incoming_event {
  int type;               /* incomming event_type */
  int subtype;            /* event_subtype */
  struct oak_dict args;   /* incomming event arguments */
};

struct outgoing_event {
  int type;               /* outgoing event_type */
  struct oak_dict args;   /* outgoing event arguments */
};

#ifdef __cplusplus
}
#endif

#endif  /* OAK_ADDONS_PUBLIC_EVENT_DEFINE_H_ */
