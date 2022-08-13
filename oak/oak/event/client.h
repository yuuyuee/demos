// Copyright 2022 The Oak Authors.

#ifndef OAK_EVENT_CHANNEL_H_
#define OAK_EVENT_CHANNEL_H_

#include <string>
#include <memory>
#include "oak/common/asio_wrapper.h"

namespace oak {

using Local = asio::local::stream_protocol;
using Internet = asio::ip::tcp;

// Client

template <typename Protocol = Internet>
class Client {
 public:
  using endpoint_type = typename Protocol::endpoint;

  Client(asio::io_context* ioctx, endpoint_type const& endpoint)
      : remote_endpoint_(endpoint), socket_(*ioctx, endpoint) {}

 private:
  typename Protocol::endpoint remote_endpoint_;
  typename Protocol::socket socket_;
};


}  // namespace oak

#endif  // OAK_EVENT_CHANNEL_H_
