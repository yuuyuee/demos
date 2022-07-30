// Copyright 2022 The Oak Authors.

#ifndef OAK_ADDONS_VERSION_H_
#define OAK_ADDONS_VERSION_H_

#define OAK_VERSION_MAJOR 1
#define OAK_VERSION_MINOR 0
#define OAK_VERSION_PATCH 0

#define OAK_VERSION               \
    ((OAK_VERSION_MAJOR << 16) |  \
    (OAK_VERSION_MINOR << 8)   |  \
    (OAK_VERSION_PATCH))


#define OAK_STRINGIFY_IMPL(x) #x
#define OAK_STRINGIFY(x) OAK_STRINGIFY_IMPL(x)

#define OAK_VERSION_STR                   \
    OAK_TO_STRING(OAK_VERSION_MAJOR) "."  \
    OAK_TO_STRING(OAK_VERSION_MINOR) "."  \
    OAK_TO_STRING(OAK_VERSION_PATCH)

#endif  /* OAK_ADDONS_VERSION_H_ */
