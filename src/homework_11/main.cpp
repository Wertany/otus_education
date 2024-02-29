#include "fmt/base.h"
#include "server.hpp"

int main(int argc, char *argv[]) {
  uint16_t port;

  if (argc != 2) {
    port = 5000;
    fmt::print("Server port: {}\n", port);
  } else {
    uint16_t port = std::atoi(argv[1]);
    fmt::print("Server port: {}\n", port);
  }

  try {
    boost::asio::io_service ios;
    Server s(ios, port);
    ios.run();
  } catch (std::exception &e) {
    fmt::print("{}", e.what());
  }
}