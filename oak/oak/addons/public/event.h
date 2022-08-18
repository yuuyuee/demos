/* Copyright 2022 The Oak Authors. */

#ifndef OAK_ADDONS_PUBLIC_EVENT_H_
#define OAK_ADDONS_PUBLIC_EVENT_H_

#include "oak/addons/public/version.h"
#include "oak/addons/public/dict.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Sink module flags */
#define OAK_MODULE_EVENT 0x04

enum event_type {
  ET_ADD_PARSER,
  ET_UPDATE_PARSER,
  ET_REMOVE_PARSER,
};

#ifndef OAK_PATH_MAX
# define OAK_PATH_MAX 255
#endif

struct event {
  int id;                   /* parser id */
  int type;                 /* event type */
  int proto_type;           /* protocol type */
  char name[OAK_PATH_MAX];  /* parser name */
  char path[OAK_PATH_MAX];  /* parser path */
};

/* struct oak_event_module
 *
 * Event module abstract interface.*/

struct oak_event_module {
  int version;        /* MUST equal to OAK_VERSION */
  int flags;          /* Module flags */

  /* Callback to initialize the module context.
   *
   * @module module context.
   * @config key/value dict that import from configuration and
   *         may used to initialize.
   *
   * Return module context on success or NULL if an error occurred. */
  void* (*init)(const struct oak_dict* config);

  /* Callback to receive the event from module.
   *
   * @module module context.
   * @event event has been received.
   * Return 0 on success or -1 if an error occurred. */
  int (*recv)(void* context, struct event* event);

  /* Callback to receive the all of event from module.
   *
   * @module module context.
   * @event some event has been received.
   * @size how many events can be received.
   * Return 0 on success or -1 if an error occurred. */
  int (*recv_bulk)(void* context, struct event* event, int size);

  /* Callback to send the event to module.
   *
   * @module module context.
   * @buffer event has been sended.
   * Return 0 on success or -1 if an error occurred. */
  int (*send)(void* context, const struct oak_buffer* buffer);

  /* Callback to close the module context. */
  void (*close)(void* context);
};

#ifdef __cplusplus
}
#endif

#endif  /* OAK_ADDONS_PUBLIC_EVENT_H_ */
