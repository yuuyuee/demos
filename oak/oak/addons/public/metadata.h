/* Copyright 2022 The Oak Authors. */

#ifndef OAK_ADDONS_PUBLIC_METADATA_H_
#define OAK_ADDONS_PUBLIC_METADATA_H_

#include "oak/addons/public/buffer.h"
#include "oak/addons/public/dict.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OAK_ID_MAX 10

/* Metadata flags */
#define MD_METRICS_INFLOW     0x00000001U
#define MD_METRICS_OUTDATA    0x00000002U

#define MD_ENABLE_COMM        0x00000004U
#define MD_METRICS_KEEPFLOW   0x00000008U

#define MD_RAW_PACKET         0x00000010U


#define OAK_TASK_ID_MAX 10
struct oak_metadata {
  int task_id[OAK_TASK_ID_MAX];
  int num_task_id;
  struct oak_buffer up_stream;
  struct oak_buffer down_stream;
  struct oak_buffer attachment;
  struct oak_dict common;
  struct oak_dict fields;
  struct oak_dict exts;
  int flags;
};

#ifdef __cplusplus
}
#endif

#endif  /* OAK_ADDONS_PUBLIC_METADATA_H_ */
