/* Copyright 2022 The Oak Authors. */

#ifndef OAK_ADDONS_PUBLIC_PARSER_H_
#define OAK_ADDONS_PUBLIC_PARSER_H_

#include "oak/addons/public/version.h"
#include "oak/addons/public/buffer.h"
#include "oak/addons/public/dict.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef OAK_NAME_MAX
# define OAK_NAME_MAX 128
#endif

/* struct oak_parser_module
 *
 * Parser module abstract interface.*/

struct oak_parser_module {
  char name[OAK_NAME_MAX];    /* Module name */
  int version;                /* MUST equal to OAK_VERSION */
  int flags;                  /* Module flags */
  void* opaque;               /* Module private data */

  /* Callback to initialize the module once before
   * any functions below has been called.
   *
   * @module module object.
   * @config key/value dict that import from configuration and
   *         may used to initialize.
   * Return 0 on success or -1 if an error occurred. */
  int (*init)(struct oak_parser_module* module,
              const struct oak_dict* config);

  /* Callback to parse the stream.
   *
   * @module module object.
   * @up_stream buffer reference the up stream.
   * @down_stream buffer reference the down stream.
   * @fields to save any pared the key/value fields.
   * Return 0 on success or -1 if an error occurred. */
  int (*parse)(struct oak_parser_module* module,
               const struct oak_buffer_ref* up_stream,
               const struct oak_buffer_ref* down_stream,
               struct oak_dict* fields);

  /* Callback to parse the stream.
   *
   * @module module object.
   * @up_stream buffer reference the up stream.
   * @down_stream buffer reference the down stream.
   * Return 0 on success or -1 if an error occurred. on success the
   * up/down stream will be saved. */
  int (*mark)(struct oak_parser_module* module,
              const struct oak_buffer_ref* up_stream,
              const struct oak_buffer_ref* down_stream);

  /* Free the module. */
  void (*free)(struct oak_parser_module* module);
};

#ifdef __cplusplus
}
#endif

#endif  /* OAK_ADDONS_PUBLIC_PARSER_H_ */
