/* Copyright 2022 The Oak Authors. */

#ifndef OAK_ADDONS_PUBLIC_EVENT_H_
#define OAK_ADDONS_PUBLIC_EVENT_H_

#include "oak/addons/public/version.h"
#include "oak/addons/public/dict.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Event module flags */
#define OAK_MODULE_EVENT 0x04

enum event_type {
  /* below for incomming event */
  ET_ADD,     /*< this option in 'oak_event_module.pull' indicate */
              /*  that parser has been enabled. */
  ET_UPDATE,
  ET_REMOVE,
  ET_CLEAR,

  /* below for outgoing event */
  ET_ACK = 1000,
  ET_ALARM,
  ET_METRICS
};

#ifndef OAK_PATH_MAX
# define OAK_PATH_MAX 255
#endif

#ifndef OAK_NAME_MAX
# define OAK_NAME_MAX 128
#endif

struct incoming_event {
  int type;                       /* incoming event type */

  /* below for parser common arguments */

  int id;                         /* parser id */
  int proto_type;                 /* protocol type */
  char proto_name[OAK_NAME_MAX];  /* protocol name */
  char file_name[OAK_PATH_MAX];   /* parser file name */
  char http_url[OAK_PATH_MAX];    /* parser http URL for download */
  char _reverse1[128];

  /* below for protocol parser arguments */

  int m_input_flow;   /* metrics: count the size of the input netflow */
  int m_output_data;  /* metrics: count the number of the output records */

  char _reverse2[128];

  /* below for data control arguments */

  int is_extract_netflow; /* enable communication */
  int m_keep_flow;  /* metrics: count the size of the controlled netflow */

  char _reverse3[128];
};

#ifndef OAK_MODULE_MAX
# define OAK_MODULE_MAX 64
#endif

#ifndef OAK_SUBJECT_MAX
# define OAK_SUBJECT_MAX 256
#endif

struct outgoing_event {
  int type;                           /* outgoing event type */
  char _reverse[128];

  union {
    struct {                          /* alarm arguments */
      char module[OAK_MODULE_MAX];    /* alarm module name */
      char subject[OAK_SUBJECT_MAX];  /* alarm subject */
      char _reverse1[128];
    } alarm;

    struct {                          /* metrics arguments */
      char module[OAK_MODULE_MAX];    /* metrics module name */
      char subject[OAK_SUBJECT_MAX];  /* metrics subject */
      char _reverse2[128];
    } metrics;

    struct {
      char _reverse3[512];
    } ack;
  };
};

/* struct oak_event_module
 *
 * Event module abstract interface.*/

struct oak_event_module {
  int version;    /* MUST equal to OAK_VERSION */
  int flags;      /* Module flags, MUST equal to OAK_MODULE_EVENT */

  /* Callback to initialize the module context.
   *
   * @module module context.
   * @config key/value dict that import from configuration and
   *         may used to initialize.
   *
   * Return 0 on success or -1 if an error occurred. */
  int (*init)(void** context, const struct oak_dict* config);

  /* Callback to pull all of previous event from module.
   *
   * @module module context.
   * @event some event has been received.
   * @size how many events can be received.
   *
   * Return greater than or equal to 0 on success indicate the number
   * of the event has been received, or -1 if an error occurred. */
  int (*pull)(void* context, struct incoming_event* event, int size);

  /* Callback to receive the event from module.
   *
   * @module module context.
   * @event event has been received.
   *
   * Return 0 on success or -1 if an error occurred. */
  int (*recv)(void* context, struct incoming_event* event);

  /* Callback to send the event to module.
   *
   * @module module context.
   * @event event has been sended.
   *
   * Return 0 on success or -1 if an error occurred. */
  int (*send)(void* context, const struct outgoing_event* event);

  /* Callback to close the module context. */
  void (*close)(void* context);
};

#ifdef __cplusplus
}
#endif

#endif  /* OAK_ADDONS_PUBLIC_EVENT_H_ */
