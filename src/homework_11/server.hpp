#ifndef SRC_HOMEWORK_11_SERVER_HPP
#define SRC_HOMEWORK_11_SERVER_HPP

#include "session.hpp"
#include <cstddef>

class Server {
public:
  Server(boost::asio::io_service &ios, std::size_t port);

  void HandleAccept(std::shared_ptr<Session> s,
                     const boost::system::error_code &e);

private:
  void AsyncAccept(std::shared_ptr<Session> s);

  boost::asio::io_service &ios_;
  boost::asio::ip::tcp::acceptor acceptor_;
};

#endif /* SRC_HOMEWORK_11_SERVER_HPP */
