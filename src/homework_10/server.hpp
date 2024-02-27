#ifndef SRC_HOMEWORK_10_SERVER_HPP
#define SRC_HOMEWORK_10_SERVER_HPP

#include <asio.hpp>
#include <cstdio>
#include <iostream>

#include "async.hpp"

class AsyncHelper {
 public:
  explicit AsyncHelper(std::size_t count_commands)
      : handle_(homework_9::async::connect(count_commands)) {}

  AsyncHelper(const AsyncHelper &) = delete;
  AsyncHelper &operator=(const AsyncHelper &) = delete;
  AsyncHelper &operator=(AsyncHelper &&) = default;
  AsyncHelper(AsyncHelper &&) noexcept = default;

  ~AsyncHelper() { homework_9::async::disconnect(handle_); }

  void receive(const char *data, std::size_t size) {
    homework_9::async::receive(handle_, data, size);
  }

 private:
  homework_9::async::handle_t handle_;
};

inline asio::awaitable<void> SessionReader(asio::ip::tcp::socket socket,
                                                  std::size_t count_commands) {
  try {
    auto client_socket = std::move(socket);
    auto async_handler = AsyncHelper{count_commands};
    for (std::string read_msg;;) {
      std::size_t n = co_await asio::async_read_until(
          client_socket, asio::dynamic_buffer(read_msg, 1024), "\n",
          asio::use_awaitable);

      async_handler.receive(read_msg.c_str(), n - 1);
      read_msg.erase(0, n);
    }
  } catch (std::exception &e) {
    std::cerr << "exception " << e.what() << '\n';
  }
}

inline asio::awaitable<void> Listener(asio::io_context &executor,
                                             std::uint16_t port,
                                             std::size_t count_commands) {
  asio::ip::tcp::acceptor acceptor(executor, {asio::ip::tcp::v6(), port});
  for (;;) {
    asio::ip::tcp::socket socket =
        co_await acceptor.async_accept(asio::use_awaitable);
    asio::co_spawn(executor, SessionReader(std::move(socket), count_commands),
                          asio::detached);
  }
}

#endif /* SRC_HOMEWORK_10_SERVER_HPP */
