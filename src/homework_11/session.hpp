#ifndef SRC_HOMEWORK_11_SESSION_HPP
#define SRC_HOMEWORK_11_SESSION_HPP

#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <optional>

class Session : public std::enable_shared_from_this<Session> {
 private:
  struct Record {
    uint64_t id;
    std::string name;
  };

  using Table = std::vector<Record>;

  struct SessionContext {
    std::string error_message;
    std::string answer;
  };

 public:
  explicit Session(boost::asio::io_service &ios);

  boost::asio::ip::tcp::tcp::socket &GetSocket();
  void AsyncRead();
  void HandleRead(std::shared_ptr<Session> &s, const boost::system::error_code &e,
                  size_t bytes);

 private:
  bool ExecuteCommand(SessionContext &ctx, const std::string &cmd);

  bool ExecuteInsert(SessionContext &ctx, const std::vector<std::string> &insert_vec);
  bool ExecuteTruncate(SessionContext &ctx, const std::string &table_name);
  bool ExecuteIntersection(SessionContext &ctx);
  bool ExecuteSymmetricDifference(SessionContext &ctx);

  bool ExecuteSelect(SessionContext &ctx, const std::string &table_name);
  bool ExecuteSelectAll(SessionContext &ctx);
  bool ExecuteDelete(SessionContext &ctx, const std::string &table_name,
                      const std::string &id_str);

  static std::optional<uint64_t> StringToUint64(SessionContext &ctx, const std::string &s);
  Session::Table *GetTable(const std::string &table_name, SessionContext *ctx = nullptr);
  static bool ExistsId(const Table &table, uint64_t id);
  std::string GetName(const std::string &table_name, uint64_t id);

  boost::asio::ip::tcp::socket socket_;
  std::vector<char> data_;
  std::unordered_map<std::string, Table> database_;
};

//-----------------------------------------------------------------------------


#endif /* SRC_HOMEWORK_11_SESSION_HPP */
