// Copyright 2022 The Oak Authors.

#ifndef OAK_COMMON_ERROR_H_
#define OAK_COMMON_ERROR_H_

#define ERROR_MAP(XX) \
  XX(SUCCESS, "Success")

namespace oak {

enum ErrorCode {
  Min = -1,

  MAX
};

}  // namespace oak

#endif  // OAK_COMMON_ERROR_H_
