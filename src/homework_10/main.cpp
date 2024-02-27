#include <csignal>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <stdexcept>

#include "fmt/base.h"
#include "server.hpp"

int main(int argc, char *argv[]) {
  uint16_t port;
  std::size_t count_commands;

  if (argc != 3) {
    port = 9012;
    count_commands = 3;
    fmt::print("Port: {}, count commands {}\n", port, count_commands);
  } else {
    uint16_t port = std::atoi(argv[1]);
    std::size_t count_commands = std::atoi(argv[2]);
    fmt::print("Port: {}, count commands {}\n", port, count_commands);
  }


  try {
    asio::io_context context(1);
    asio::signal_set signals(context, SIGINT, SIGTERM);
    signals.async_wait([&](auto, auto) { context.stop(); });

    asio::co_spawn(context, Listener(context, port, count_commands), asio::detached);

    context.run();
  } catch (const std::runtime_error &e) { fmt::print("Error: {}", e.what()); }
}