/* Copyright 2022 The Oak Authors. */

#ifndef OAK_ADDONS_PUBLIC_SINK_H_
#define OAK_ADDONS_PUBLIC_SINK_H_

#include "oak/addons/public/compiler.h"
#include "oak/addons/public/platform.h"
#include "oak/addons/public/meta.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef OAK_NAME_MAX
# define OAK_NAME_MAX 128
#endif

/* Sink module flags */
#define OAK_MODULE_SINK 0x03

/* struct oak_sink_module
 *
 * Sink module abstract interface.*/

struct oak_sink_module {
  char name[OAK_NAME_MAX];    /* Module name */
  int version;                /* MUST equal to OAK_VERSION */
  int flags;                  /* Module flags */
  void* priv_data;            /* Module private data */

  /* Callback to initialize the module once before any functions
   * below has been called.
   *
   * @module module object.
   * @config key/value dict that import from configuration and
   *         may used to initialize.
   *
   * Return 0 on success or -1 if an error occurred. */
  int (*init)(struct oak_sink_module* module,
              const struct oak_dict* config);

  /* Callback to write the metadata to module specified the sink.
   *
   * @module module object.
   * @meta some key/value fileds has been saved.
   *
   * Return 0 on success or -1 if an error occurred. */
  int (*write)(struct oak_sink_module* module,
               const struct oak_metadata* metadata);

  /* Callback to close the module. */
  void (*close)(struct oak_sink_module* module);
};

#ifdef __cplusplus
}
#endif

#endif  /* OAK_ADDONS_PUBLIC_SINK_H_ */
