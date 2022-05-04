// Copyright 2021 bartem1us filkovghjynj@mail.ru
#ifndef TEMPLATE_SERVER_H
#define TEMPLATE_SERVER_H

#include "connect.h"

class Server {
  using tcp = boost::asio::ip::tcp;
  using error_code = boost::beast::error_code;
  using io_context = boost::asio::io_context;

 private:
  io_context io_context_;
  tcp::acceptor acceptor_;
  std::shared_ptr<Handler> serv_handler;

 public:
  Server(const std::string& host, const std::string& port);

  void RunServer();

 private:
  void StartAccept();

  void ApplyAccept(error_code err, tcp::socket socket_);

  void ApplyStop();
};

#endif  // TEMPLATE_SERVER_H
