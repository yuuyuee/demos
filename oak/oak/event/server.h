// Copyright 2022 The Oak Authors.

#ifndef OAK_EVENT_SERVER_H_
#define OAK_EVENT_SERVER_H_

#include <unistd.h>

#include <string>
#include <memory>
#include <utility>
#include <set>
#include <array>
#include <system_error>
#include <type_traits>

#include "oak/event/request.h"
#include "oak/common/asio_wrapper.h"
#include "oak/logging/logging.h"

namespace oak {

using Local = asio::local::stream_protocol;
using Internet = asio::ip::tcp;

template <typename>
class SessionManager;

// Session

template <typename Protocol = Internet>
class Session: public std::enable_shared_from_this<Session<Protocol>> {
 public:
  using Pointer = std::shared_ptr<Session>;
  using Socket = typename Protocol::socket;

  static const size_t kMaxMessageSize = 8192;

  // Construct a session with the given socket and handler.
  explicit Session(Socket socket,
                   SessionManager<Protocol>* manager_);

  // Start the session with a asynchronous read.
  void Start();

  // Stop all of asynchronous operations associated with session.
  void Stop();

 private:
  void AsyncRead();
  void AsyncWrite(size_t size);

 private:
  Session(const Session&) = delete;
  Session& operator=(const Session&) = delete;

  std::array<char, kMaxMessageSize> buffer_;
  Socket socket_;
  SessionManager<Protocol>* session_manager_;
};

template <typename Protocol>
Session<Protocol>::Session(Socket socket,
                           SessionManager<Protocol>* manager)
    : buffer_(), socket_(std::move(socket)), session_manager_(manager) {}

template <typename Protocol>
void Session<Protocol>::Start() {
  AsyncRead();
}

template <typename Protocol>
void Session<Protocol>::Stop() {
  socket_.close();
}

template <typename Protocol>
void Session<Protocol>::AsyncRead() {
  auto self(this->shared_from_this());
  socket_.async_read_some(asio::buffer(buffer_),
      [this, self](std::error_code ec, size_t bytes) {
        if (ec) {
          if (ec != asio::error::operation_aborted)
            session_manager_->Stop(self);
          return;
        }

        OAK_INFO("Receive: %*s\n", static_cast<int>(bytes), buffer_.data());
        AsyncWrite(bytes);
      });
}

template <typename Protocol>
void Session<Protocol>::AsyncWrite(size_t size) {
  auto self(this->shared_from_this());
  asio::async_write(socket_, asio::const_buffer(buffer_.data(), size),
      [this, self](std::error_code ec, size_t) {
        if (ec) {
          if (ec != asio::error::operation_aborted)
            session_manager_->Stop(self);
          return;
        }

        // std::error_code ignored_ec;
        // socket_.shutdown(Socket::shutdown_both, ignored_ec);
        AsyncRead();
      });
}

// SessionManager
// Note: this class is not thread-safty, but here shall be only used
// in single thread.

template <typename Protocol = Internet>
class SessionManager {
 public:
  using SessionPointer = typename Session<Protocol>::Pointer;

  static const size_t KMaxSessions = 10000;

  SessionManager();

  // Add the specified session to the manager and start it.
  void Start(SessionPointer session);

  // Stop the specified session.
  void Stop(SessionPointer session);

  // Stop all of the session.
  void StopAll();

 private:
  std::set<SessionPointer> sessions_;
};

template <typename Protocol>
SessionManager<Protocol>::SessionManager(): sessions_() {}

template <typename Protocol>
void SessionManager<Protocol>::Start(SessionPointer session) {
  sessions_.insert(session);
  session->Start();
}

template <typename Protocol>
void SessionManager<Protocol>::Stop(SessionPointer session) {
  sessions_.erase(session);
  session->Stop();
}

template <typename Protocol>
void SessionManager<Protocol>::StopAll() {
  for (auto session : sessions_)
    session->Stop();
  sessions_.clear();
}

// Server

template <typename Protocol = Internet>
class Server {
 public:
  using EndpointType = typename Protocol::endpoint;
  using Acceptor = typename Protocol::acceptor;
  using Socket = typename Protocol::socket;

  explicit Server(asio::io_context* ioctx);

  ~Server() {}

  void Start(const std::string& address, const std::string& port);
  void Stop();

 private:
  Server(const Server&) = delete;
  Server& operator=(const Server&) = delete;

  void Bind(const std::string& address,
            const std::string& port,
            typename Internet::endpoint* = 0);

  void Bind(const std::string& address,
            const std::string& port,
            typename Local::endpoint* = 0);

  void AsyncAccept();

  // EndpointType endpoint_;
  Acceptor acceptor_;
  SessionManager<Protocol> session_manager_;
};

template <typename Protocol>
Server<Protocol>::Server(asio::io_context* ioctx)
    : acceptor_(*ioctx), session_manager_() {}

template <typename Protocol>
void Server<Protocol>::Start(const std::string& address,
                             const std::string& port) {
  Bind(address, port, reinterpret_cast<EndpointType*>(0));
  acceptor_.listen();
  AsyncAccept();
}

template <typename Protocol>
void Server<Protocol>::Bind(const std::string& address,
                            const std::string& port,
                            typename Internet::endpoint*) {
  typename Protocol::resolver resolver(acceptor_.get_executor());
  auto result = resolver.resolve(address, port);
  if (result.empty())
    OAK_FATAL("Unable address resolve\n");
  typename Internet::endpoint endpoint = *result.begin();
  acceptor_.open(endpoint.protocol());
  acceptor_.set_option(typename Acceptor::reuse_address(true));
  acceptor_.bind(endpoint);
}

template <typename Protocol>
void Server<Protocol>::Bind(const std::string& address,
                                       const std::string&,
                                       typename Local::endpoint*) {
  unlink(address.c_str());
  typename Local::endpoint endpoint = typename Local::endpoint(address);
  acceptor_.open(endpoint.protocol());
  acceptor_.bind(endpoint);
}

template <typename Protocol>
void Server<Protocol>::AsyncAccept() {
  acceptor_.async_accept(
      [this](std::error_code ec, Socket socket) {
        if (!acceptor_.is_open())
          return;

        if (!ec) {
          auto session = std::make_shared<Session<Protocol>>(
              std::move(socket), &session_manager_);
          session_manager_.Start(session);
        } else {
          throw std::system_error(ec);
        }

        AsyncAccept();
      });
}

}  // namespace oak

#endif  // OAK_EVENT_SERVER_H_
