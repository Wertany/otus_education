#include "server.hpp"
#include "fmt/base.h"

Server::Server(boost::asio::io_service &ios, std::size_t port)
    : ios_(ios), acceptor_(ios, boost::asio::ip::tcp::endpoint(
                                    boost::asio::ip::tcp::v4(), port)) {
  AsyncAccept(std::make_shared<Session>(ios_));
}

void Server::HandleAccept(std::shared_ptr<Session> s,
                          const boost::system::error_code &e) {
  if (e) {
    fmt::print("Can't accept connect from client: {}\n", e.message());
    s.reset();
    return;
  }

  auto rmt = s->GetSocket().remote_endpoint();
  fmt::print("Accepted connection from {}:{}\n", rmt.address().to_string(),
             rmt.port());

  s->AsyncRead();
  s = std::make_shared<Session>(ios_);
  AsyncAccept(s);
}

void Server::AsyncAccept(std::shared_ptr<Session> s) { // NOLINT
  acceptor_.async_accept(s->GetSocket(),
                         boost::bind(&Server::HandleAccept, this, s, // NOLINT
                                     boost::asio::placeholders::error));
}
