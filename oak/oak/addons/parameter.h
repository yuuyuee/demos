// Copyright 2022 The Oak Authors.

#ifndef OAK_ADDONS_PARAMETER_H_
#define OAK_ADDONS_PARAMETER_H_

#include <sys/types.h>
#include <stdlib.h>
#include <assert.h>
#include <stddef.h>

#include "oak/addons/internal/compiler.h"
#include "oak/addons/internal/platform.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef NAME_MAX
# define NAME_MAX 255
#endif

#ifndef PATH_MAX
# define PATH_MAX 4096
#endif

#ifdef __cplusplus
}
#endif

#endif  /* OAK_ADDONS_PARAMETER_H_ */
