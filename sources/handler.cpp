// Copyright 2021 bartem1us filkovghjynj@mail.ru
#include "handler.h"

#include <iostream>
#include <thread>

#include "fstream"

Handler::Handler() {
  std::thread([this]() { Update(); }).detach();
}

json Handler::GetSuggest(const std::string& input) noexcept {
  size_t pos = 0;
  json find_data;
  json element;
  for (size_t i = 0; i < suggestions.size(); ++i) {
    if (suggestions[i]["id"] == input)  //
    {
      element["text"] = suggestions[i]["name"];
      element["position"] = pos;
      ++pos;
      find_data["suggestions"].push_back(element);
    }
  }
  if (pos == 0) {
    find_data["suggestions"] = json::array();
  }
  return find_data;
}

[[noreturn]] void Handler::Update() noexcept {
  while (true) {
    std::shared_lock<std::shared_mutex> lock;
    std::ifstream in("../suggestions.json");
    in >> suggestions;
    std::sort(suggestions.begin(), suggestions.end(),
              [](const json& first, const json& second) {
                return first["cost"] < second["cost"];
              });
    std::cout << "after Update" << std::endl;
    std::this_thread::sleep_for(std::chrono::minutes(15));
  }
}

Response Handler::GetResponse(const Request& request) {
  try {
    if (request.method() == http::verb::post) {
      std::shared_lock<std::shared_mutex> lock(mtx);
      json data = json::parse(request.body());

      if (data.count("input") && data.at("input").is_string()) {
        json suggest = GetSuggest(data.at("input"));
        Response response;  //Формируем запрос
        response.result(http::status::ok);
        response.set(http::field::content_type,
                     "application/json");                      // endpoint
        response.content_length(to_string(suggest).length());  // answer length
        response.body() = to_string(suggest);  // answer -> body of answer
        response.version(request.version());
        return response;
      }
    }
    Response response{http::status::method_not_allowed,
                      request.version()};  // if not post request
    return response;
  } catch (json::exception& err) {
    Response response{http::status::bad_request, request.version()};
    return response;
  } catch (std::exception& err) {
    Response response{http::status::internal_server_error, request.version()};
    return response;
  }
}
