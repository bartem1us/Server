// Copyright 2021 bartem1us filkovghjynj@mail.ru
#include "server.h"

int main() {
  Server server("0.0.0.0", "3030");

  server.RunServer();

}
