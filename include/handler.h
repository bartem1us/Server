// Copyright 2021 bartem1us filkovghjynj@mail.ru
#ifndef TEMPLATE_HANDLER_H
#define TEMPLATE_HANDLER_H
#include <boost/beast/http.hpp>
#include <nlohmann/json.hpp>
#include <shared_mutex>
#include <string>

using nlohmann::json;
namespace http = boost::beast::http;
using Request = http::request<http::string_body>;    // zapros
using Response = http::response<http::string_body>;  // otvet
class Handler {
  json suggestions;

  mutable std::shared_mutex mtx;

 public:
  json GetSuggest(const std::string& input) noexcept;  // find with string

  [[noreturn]] void Update() noexcept;

  Response GetResponse(const Request& request);

  Handler();
};
#endif  // TEMPLATE_HANDLER_H
