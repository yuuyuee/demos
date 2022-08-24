/* Copyright 2022 The Oak Authors. */

#ifndef OAK_ADDONS_PUBLIC_EVENT_H_
#define OAK_ADDONS_PUBLIC_EVENT_H_

#include "oak/addons/public/version.h"
#include "oak/addons/public/dict.h"
#include "oak/addons/public/event_define.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Event module flags */
#define OAK_MODULE_EVENT 0x04

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
