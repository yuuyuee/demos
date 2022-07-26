// Copyright 2022 The Oak Authors.

#include "oak/version.h"

#define OAK_TO_STRING_IMPL(x) #x
#define OAK_TO_STRING(x) OAK_TO_STRING_IMPL(x)
#define OAK_VERSION_STR_BASE OAK_TO_STRING(OAK_VERSION_MAJOR) "." \
                             OAK_TO_STRING(OAK_VERSION_MINOR) "." \
                             OAK_TO_STRING(OAK_VERSION_PATCH)

#if OAK_VERSION_IS_RELEASE
# define OAK_VERSION_STR OAK_VERSION_STR_BASE
#else
# define OAK_VERSION_STR OAK_VERSION_STR_BASE "-" OAK_VERSION_SUFFIX
#endif

unsigned int oak_version() {
  return OAK_VERSION;
}

const char* oak_version_str() {
  return OAK_VERSION_STR;
}
