// Copyright 2021 bartem1us filkovghjynj@mail.ru
#include "server.h"

Server::Server(const std::string &host, const std::string &port)
    : acceptor_(boost::asio::make_strand(io_context_)),
      serv_handler(std::make_shared<Handler>()) {
  tcp::resolver resolver_(io_context_);

  tcp::endpoint endpoint_ = *resolver_.resolve(host, port).begin();

  acceptor_.open(endpoint_.protocol());

  acceptor_.set_option(boost::asio::socket_base::reuse_address(true));

  acceptor_.bind(endpoint_);

  acceptor_.listen();
}

void Server::RunServer() {
  StartAccept();

  io_context_.run();
}

void Server::StartAccept() {
  acceptor_.async_accept(
      boost::asio::make_strand(io_context_),
      boost::beast::bind_front_handler(&Server::ApplyAccept, this));
}

void Server::ApplyAccept(error_code err, tcp::socket socket_) {
  if (!err) {
    auto connection =
        std::make_shared<Connect>(std::move(socket_), serv_handler);
    connection->Open();
  }
  StartAccept();
}

void Server::ApplyStop() { io_context_.stop(); }
