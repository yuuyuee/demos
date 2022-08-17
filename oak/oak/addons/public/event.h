/* Copyright 2022 The Oak Authors. */

#ifndef OAK_ADDONS_PUBLIC_EVENT_H_
#define OAK_ADDONS_PUBLIC_EVENT_H_

#include "oak/addons/public/version.h"
#include "oak/addons/public/buffer.h"
#include "oak/addons/public/dict.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Sink module flags */
#define OAK_MODULE_EVENT 0x04

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
  int (*init)(const struct oak_dict* config);

  /* Callback to receive the event from module.
   *
   * @module module context.
   * @buffer some event has been received.
   * Return 0 on success or -1 if an error occurred. */
  int (*receive)(void* context, struct oak_buffer* buffer);

  /* Callback to report the event to module.
   *
   * @module module context.
   * @buffer some event has been reported.
   * Return 0 on success or -1 if an error occurred. */
  int (*report)(void* context, const struct oak_buffer* buffer);

  /* Callback to close the module context. */
  void (*close)(void* context);
};

#ifdef __cplusplus
}
#endif

#endif  /* OAK_ADDONS_PUBLIC_EVENT_H_ */
