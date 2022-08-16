/* Copyright 2022 The Oak Authors. */

#ifndef OAK_ADDONS_PUBLIC_SOURCE_H_
#define OAK_ADDONS_PUBLIC_SOURCE_H_

#include "oak/addons/public/buffer.h"
#include "oak/addons/public/dict.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef OAK_NAME_MAX
# define OAK_NAME_MAX 128
#endif

/* Source module flags */
#define OAK_MODULE_SOURCE 0x01

/* struct oak_source_module
 *
 * Source module abstract interface. */

struct oak_source_module {
  char name[OAK_NAME_MAX];    /* Module name */
  int version;                /* MUST equal to OAK_VERSION */
  int flags;                  /* Module flags */
  void* priv_data;            /* Module private data */

  /* Callback to initialize the object once before any functions
   * below has been called.
   *
   * @module module object.
   * @config key/value dict that import from configuration and
   *         may used to initialize.
   *
   * Return 0 on success or -1 if an error occurred. */
  int (*init)(struct oak_source_module* module,
              const struct oak_dict* config);

  /* Callback to read the buffer and decoded as the metadata.
   *
   * @module module object.
   * @buffer buffer shoul be read from.
   *
   * Return 0 on success or -1 if an error occurred. */
  int (*read)(const struct oak_source_module* module,
              const struct oak_buffer_ref* buffer,
              struct oak_metadata* metadata);

  /* Callback to close the module. */
  void (*close)(struct oak_source_module* module);
};

#ifdef __cplusplus
}
#endif

#endif  /* OAK_ADDONS_PUBLIC_SOURCE_H_ */
