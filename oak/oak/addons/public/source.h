/* Copyright 2022 The Oak Authors. */

#ifndef OAK_ADDONS_PUBLIC_SOURCE_H_
#define OAK_ADDONS_PUBLIC_SOURCE_H_

#include "oak/addons/public/version.h"
#include "oak/addons/public/dict.h"
#include "oak/addons/public/metadata.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Source module flags */
#define OAK_MODULE_SOURCE 0x01

/* struct oak_source_module
 *
 * Source module abstract interface. */

struct oak_source_module {
  int version;        /* MUST equal to OAK_VERSION */
  int flags;          /* Module flags */

  /* Callback to initialize the module context.
   *
   * @config key/value dict that import from configuration and
   *         may used to initialize.
   *
   * Return module context, this function always success no matter
   * what value is returned. */
  void* (*init)(const struct oak_dict* config);

  /* Callback to read the buffer and decoded as the metadata.
   *
   * @context module context.
   * @metadata read from module.
   *
   * Return 0 on success or -1 if an error occurred. */
  int (*read)(void* context, struct oak_metadata* metadata);

  /* Callback to close the module context. */
  void (*close)(void* context);
};

#ifdef __cplusplus
}
#endif

#endif  /* OAK_ADDONS_PUBLIC_SOURCE_H_ */
