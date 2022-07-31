// Copyright 2022 The Oak Authors.

#ifndef OAK_COMMON_IGNORE_UNUSED_H_
#define OAK_COMMON_IGNORE_UNUSED_H_

namespace oak {
template <typename... Args>
constexpr void IgnoreUnused(Args const&...) {}

template <typename... Args>
constexpr void IgnoreUnused() {}
}  // namespace oak

#endif  // OAK_COMMON_IGNORE_UNUSED_H_
