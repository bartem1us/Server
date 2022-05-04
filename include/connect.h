// Copyright 2021 bartem1us filkovghjynj@mail.ru
#ifndef TEMPLATE_CONNECT_H
#define TEMPLATE_CONNECT_H
#include <utility>

#include "boost/asio.hpp"
#include "boost/beast/core.hpp"
#include "boost/beast/http.hpp"
#include "handler.h"

using tcp = boost::asio::ip::tcp;
using error_code = boost::beast::error_code;
using tcp_stream = boost::beast::tcp_stream;
using buffer = boost::beast::flat_buffer;
// for_safe_generation of shared ptr

class Connect : public std::enable_shared_from_this<Connect> {
  tcp_stream stream;  // socked TCP/IP with timeout
  buffer buffer_;     // dynamic bufer with changed memmory
  Request request_;
  std::shared_ptr<void> res;  // on any obj
  std::shared_ptr<Handler> handler;

 public:
  Connect(tcp::socket&& socket, const std::shared_ptr<Handler>& serv_handler)
      : stream(std::move(socket)), handler(serv_handler) {}

  void Open();

  void HandlerRead(error_code err);

  void Close();

  void DoRead();

  void HandlerWrite(bool is_closed, error_code err);
};
#endif  // TEMPLATE_CONNECT_H
