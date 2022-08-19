/* Copyright 2022 The Oak Authors. */

#ifndef OAK_ADDONS_PUBLIC_PARSER_H_
#define OAK_ADDONS_PUBLIC_PARSER_H_

#include "oak/addons/public/version.h"
#include "oak/addons/public/buffer.h"
#include "oak/addons/public/dict.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Parser module flags */
#define OAK_MODULE_PARSER 0x02

/* struct oak_parser_module
 *
 * Parser module abstract interface.*/

struct oak_parser_module {
  int version;        /* MUST equal to OAK_VERSION */
  int flags;          /* Module flags */

  /* Callback to initialize the module context.
   *
   * @module module object.
   * @config key/value dict that import from configuration and
   *         may used to initialize.
   *
   * Return module context, this function always success no matter
   * what value is returned. */
  void* (*init)(const struct oak_dict* config);

  /* Callback to parsing the stream to extract the fields.
   *
   * @module module context.
   * @up_stream buffer reference the up stream.
   * @down_stream buffer reference the down stream.
   * @fields to save any pared the key/value fields.
   *
   * Return 0 on success or -1 if an error occurred. */
  int (*parse)(void* context,
               const struct oak_buffer_ref* up_stream,
               const struct oak_buffer_ref* down_stream,
               struct oak_dict* fields);

  /* Callback to parsing the stream to indicate whether or not
   * the stream should be saved.
   *
   * @module module context.
   * @up_stream buffer reference the up stream.
   * @down_stream buffer reference the down stream.
   *
   * Return 0 on success or -1 if an error occurred. on success
   * the up/down stream should be saved. */
  int (*mark)(void* context,
              const struct oak_buffer_ref* up_stream,
              const struct oak_buffer_ref* down_stream);

  /* Callback to close the module context. */
  void (*close)(void* context);
};

#ifdef __cplusplus
}
#endif

#endif  /* OAK_ADDONS_PUBLIC_PARSER_H_ */
