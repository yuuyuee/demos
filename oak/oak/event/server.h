// Copyright 2022 The Oak Authors.

#ifndef OAK_EVENT_CHANNEL_H_
#define OAK_EVENT_CHANNEL_H_

#include <string>
#include <memory>
#include "oak/common/asio_wrapper.h"

namespace oak {

using Local = asio::local::stream_protocol;
using Internet = asio::ip::tcp;

// Session

template <typename Protocol = Internet>
class Session: public std::enable_shared_from_this<Session> {
 public:
  using pointer = std::shared_ptr<Session>;

  static pointer Create(asio::io_context* ioctx) {
    return pointer(new Session(*ioctx));
  }

  typename Protocol::socket& socket() {
    return socket_;
  }

 private:
  explicit Session(asio::io_context* ioctx): socket_(*ioctx) {}

 private:
  typename Protocol::socket socket_;
};

// Server

template <typename Protocol = Internet>
class Server {
 public:
  using endpoint_type = typename Protocol::endpoint;

  Server(asio::io_context* ioctx, endpoint_type const& endpoint)
      : endpoint_(endpoint), acceptor_(*ioctx, endpoint) {}

  void Start();

 private:
  Server(const Server&) = delete;
  Server& operator=(const Server&) = delete;

  typename Protocol::endpoint endpoint_;
  typename Protocol::acceptor acceptor_;
};

template <typename Protocol>
void Server<Protocol>::Start();


}  // namespace oak

#endif  // OAK_EVENT_CHANNEL_H_
