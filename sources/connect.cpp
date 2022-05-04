// Copyright 2021 bartem1us filkovghjynj@mail.ru
#include "connect.h"

void Connect::DoRead() {
  request_ = {};
  http::async_read(
      stream, buffer_, request_,
      boost::beast::bind_front_handler(
          [this]([[maybe_unused]] std::shared_ptr<Connect> sp,
                 boost::system::error_code& ec,
                 [[maybe_unused]] long unsigned int& i) { HandlerRead(ec); },
          shared_from_this()));
}
void Connect::HandlerRead(error_code err) {
  if (err == http::error::end_of_stream) {
    Close();
  }
  if (!err) {
    Response response;
    if (request_.target() == "/v1/api/suggest") {
      response = handler->GetResponse(request_);

    } else {
      response.set(http::field::server, "not connected");

      response.set(http::field::content_type, "application/json");

      response.result(http::status::not_found);

      response.body() = "Not found";
    }

    auto pointer = std::make_shared<http::message<false, http::string_body>>(
        std::move(response));

    res = pointer;

    http::async_write(stream, *pointer,
                      boost::beast::bind_front_handler(
                          [this]([[maybe_unused]] std::shared_ptr<Connect> sp,
                                 bool is_closed, boost::system::error_code& ec,
                                 [[maybe_unused]] long unsigned int& i) {
                            HandlerWrite(is_closed, ec);
                          },
                          shared_from_this(),
                          pointer->need_eof()));  // parametrs ->HandlerWrite
  }
}

void Connect::HandlerWrite(bool is_closed, error_code err) {
  if (is_closed) {
    return Close();
  }
  if (!err) {
    DoRead();
  }
}

void Connect::Close() {
  error_code err;
  stream.socket().shutdown(tcp::socket::shutdown_send, err);
}

void Connect::Open() {
  boost::asio::dispatch(
      stream.get_executor(),
      boost::beast::bind_front_handler(&Connect::DoRead, shared_from_this()));
}

