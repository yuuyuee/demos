/* Copyright 2022 The Oak Authors. */

#ifndef OAK_ADDONS_PUBLIC_SINK_H_
#define OAK_ADDONS_PUBLIC_SINK_H_

#include "oak/addons/public/version.h"
#include "oak/addons/public/dict.h"
#include "oak/addons/public/metadata.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Sink module flags */
#define OAK_MODULE_SINK 0x03

/* struct oak_sink_module
 *
 * Sink module abstract interface.*/

struct oak_sink_module {
  int version;        /* MUST equal to OAK_VERSION */
  int flags;          /* Module flags */

  /* Callback to initialize the module context.
   *
   * @module module context.
   * @config key/value dict that import from configuration and
   *         may used to initialize.
   *
   * Return 0 on success or -1 if an error occurred. */
  int (*init)(void** context, const struct oak_dict* config);

  /* Callback to write the metadata to module specified the sink.
   *
   * @module module context.
   * @meta some key/value fileds has been saved.
   *
   * Return 0 on success or -1 if an error occurred. */
  int (*write)(void* context,
               const struct oak_metadata* metadata);

  /* Callback to close the module context. */
  void (*close)(void* context);
};

#ifdef __cplusplus
}
#endif

#endif  /* OAK_ADDONS_PUBLIC_SINK_H_ */
