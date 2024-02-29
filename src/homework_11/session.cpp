#include "session.hpp"

#include <algorithm>
#include <cstddef>
#include <string>

#include "fmt/format.h"

namespace {
constexpr std::size_t kSizeData = 1024;

std::vector<std::string> SplitString(const std::string &s, char sep) {
  std::vector<std::string> v;

  if (!s.empty()) {
    std::size_t pos{};
    std::size_t last_pos{};
    while ((pos = s.find(sep, last_pos)) != std::string::npos) {
      if (pos != 0) {
        if ((pos - last_pos) > 0) {
          v.emplace_back(s.substr(last_pos, pos - last_pos));
        }
        last_pos = ++pos;
      } else {
        ++last_pos;
      }
    }

    if (pos == std::string::npos) {
      std::size_t size = s.size();
      if (last_pos < size) {
        v.emplace_back(s.substr(last_pos, size - last_pos));
      }
    }
  }

  return v;
}

void StringToLower(std::string &s) {
  for (char &i : s) {
    i = std::move(static_cast<char>(std::tolower(static_cast<int>(i))));
  }
}

void StringRmRight(std::string &s, size_t n) {
  size_t size = s.size();

  if (size == 0) {
    return;
  }

  if (n >= size) {
    s.clear();
  } else {
    s.erase(size - n);
  }
}
} // namespace

Session::Session(boost::asio::io_service &ios) : socket_(ios) {
  data_.resize(kSizeData);
}

boost::asio::ip::tcp::socket &Session::GetSocket() { return socket_; }

void Session::AsyncRead() {
  socket_.async_read_some(
      boost::asio::buffer(data_, kSizeData),
      boost::bind(&Session::HandleRead, this, shared_from_this(), // NOLINT
                  boost::asio::placeholders::error,
                  boost::asio::placeholders::bytes_transferred));
}

void Session::HandleRead(std::shared_ptr<Session> &s,
                         const boost::system::error_code &e, size_t bytes) {
  auto rmt = s->GetSocket().remote_endpoint();
  std::string client_address =
      fmt::format("{}:{}", rmt.address().to_string(), rmt.port());

  if (e) {
    if (e == boost::asio::error::connection_reset ||
        e == boost::asio::error::eof) {
      fmt::print("Disconnected {}\n", client_address);
    } else {
      fmt::print("Can't read data from client {}: {}\n", client_address,
                 e.message());
    }
    return;
  }

  auto commands =
      SplitString(std::string(data_.begin(), data_.begin() + bytes), '\n');
  for (const auto &command : commands) {
    SessionContext ctx;

    bool res = ExecuteCommand(ctx, command);

    std::string answer;
    if (res) {
      answer = "OK";

      if (!ctx.answer.empty()) {
        answer += "\n" + ctx.answer;
      }
    } else {
      answer = ctx.error_message;
    }
    answer += "\n";

    s->GetSocket().write_some(boost::asio::buffer(answer, answer.size()));
  }

  std::fill(data_.begin(), data_.end(), '\0');
  AsyncRead();
}

bool Session::ExecuteCommand(SessionContext &ctx, const std::string &cmd) {
  auto v = SplitString(cmd, ' ');
  size_t v_size = v.size();
  if (v_size == 0) {
    ctx.error_message = "Invalid format!";
    return false;
  }

  std::string command_type = v.front();
  StringToLower(command_type);

  if (command_type == "select" && v_size == 2) {
    return ExecuteSelect(ctx, v[1]);
  }
  if (command_type == "selectall") {
    return ExecuteSelectAll(ctx);
  }
  if (command_type == "insert" && v_size == 4) {
    return ExecuteInsert(ctx, v);
  }
  if (command_type == "delete" && v_size == 3) {
    return ExecuteDelete(ctx, v[1], v[2]);
  }
  if (command_type == "truncate" && v_size == 2) {
    return ExecuteTruncate(ctx, v[1]);
  }
  if (command_type == "intersection" && v_size == 1) {
    return ExecuteIntersection(ctx);
  }
  if (command_type == "symmetric_difference" && v_size == 1) {
    return ExecuteSymmetricDifference(ctx);
  }

  ctx.error_message = "Invalid command: " + command_type;

  return false;
}

bool Session::ExecuteInsert(SessionContext &ctx,
                            const std::vector<std::string> &insert_vec) {
  const std::string &table_name = insert_vec[1];

  uint64_t id = 0;
  if (auto a = std::stoul(insert_vec[2]); a) {
    id = a;
  }

  const std::string &name = insert_vec[3];

  auto it = database_.find(table_name);
  if (it != database_.end()) {
    for (const auto &record : it->second) {
      if (record.id == id) {
        ctx.error_message = fmt::format("ERR duplicate {}", insert_vec[2]);
        return false;
      }
    }
    it->second.emplace_back(Record{id, name});
  } else {
    database_[table_name].emplace_back(Record{id, name});
  }

  return true;
}

bool Session::ExecuteTruncate(SessionContext &ctx,
                              const std::string &table_name) {
  auto *tbl = GetTable(table_name, &ctx);
  if (!tbl) {
    return false;
  }

  database_.erase(table_name);
  return true;
}

bool Session::ExecuteIntersection(SessionContext &ctx) {
  size_t table_count = database_.size();
  if (table_count == 1) {
    ctx.error_message = "ERR for this operator you need to have more one table";
    return false;
  }
  --table_count;

  const auto &first_table = database_.begin()->second;

  std::unordered_map<uint64_t, std::vector<std::string>> m_temp;

  for (auto it = std::next(database_.begin()); it != database_.end(); ++it) {
    for (const auto &record : first_table) {
      if (ExistsId(it->second, record.id)) {
        m_temp[record.id].emplace_back(it->first);
      }
    }
  }

  for (auto &it : m_temp) {
    if (it.second.size() != table_count) {
      continue;
    }

    ctx.answer += std::to_string(it.first) + "," +
                  GetName(database_.begin()->first, it.first) + ",";

    for (const std::string &table_name : it.second) {
      ctx.answer += GetName(table_name, it.first) + ",";
    }

    StringRmRight(ctx.answer, 1);
    ctx.answer += "\n";
  }

  return true;
}

bool Session::ExecuteSymmetricDifference(SessionContext &ctx) {
  if (database_.size() != 2) {
    ctx.error_message = "Support symmetric difference for two tables only";
    return false;
  }

  auto &table_a = database_.begin()->second;
  auto &table_b = std::next(database_.begin())->second;

  auto l_print = [&ctx](uint64_t id, const std::string &name_a,
                        const std::string &name_b) {
    ctx.answer += std::to_string(id) + "\t" + name_a + "\t" + name_b + "\n";
  };

  ctx.answer = "ID\t" + database_.begin()->first + "\t" +
               std::next(database_.begin())->first + "\n";

  for (const auto &record : table_a) {
    if (!ExistsId(table_b, record.id)) {
      l_print(record.id, record.name, std::string());
    }
  }

  for (const auto &record : table_b) {
    if (!ExistsId(table_a, record.id)) {
      l_print(record.id, std::string(), record.name);
    }
  }

  return true;
}

bool Session::ExecuteSelect(SessionContext &ctx,
                            const std::string &table_name) {
  auto *tbl = GetTable(table_name, &ctx);
  if (!tbl) {
    return false;
  }

  ctx.answer += table_name + ":\n";

  if (tbl->empty()) {
    ctx.answer += "Table \"" + table_name + "\" is empty";
  } else {
    for (const auto &record : (*tbl)) {
      ctx.answer += std::to_string(record.id) + "\t" + record.name + "\n";
    }
  }

  return true;
}

bool Session::ExecuteSelectAll(SessionContext &ctx) {
  for (const auto &table : database_) {
    if (!ExecuteSelect(ctx, table.first)) {
      return false;
    }
    ctx.answer += "\n";
  }
  return true;
}

bool Session::ExecuteDelete(SessionContext &ctx, const std::string &table_name,
                            const std::string &id_str) {
  auto *tbl = GetTable(table_name, &ctx);
  if (!tbl) {
    return false;
  }

  auto id = StringToUint64(ctx, id_str);
  if (!id) {
    return false;
  }

  bool found = false;

  for (size_t i = 0, c = tbl->size(); i < c; ++i) {
    Record &record = (*tbl)[i];
    found = record.id == id;

    if (found) {
      auto it_beg = tbl->begin();
      std::advance(it_beg, i);
      tbl->erase(it_beg);
      break;
    }
  }

  if (!found) {
    ctx.error_message = fmt::format("Record with id {} not found", id_str);
    return false;
  }

  return true;
}

std::optional<uint64_t> Session::StringToUint64(SessionContext &ctx,
                                                const std::string &s) {
  uint64_t id = 0;
  if (auto a = std::stoull(s); a) {
    id = a;
  } else {
    ctx.error_message = "Invalid ID";
    return std::nullopt;
  }

  return id;
}

Session::Table *Session::GetTable(const std::string &table_name,
                                  SessionContext *ctx) {
  auto it = database_.find(table_name);
  if (it == database_.end()) {
    if (ctx) {
      ctx->error_message = "Table \"" + table_name + "\" not found";
    }
    return nullptr;
  }
  return &it->second;
}

bool Session::ExistsId(const Table &table, uint64_t id) {
  for (const auto &record : table) { // NOLINT
    if (record.id == id) {
      return true;
    }
  }
  return false;
}

std::string Session::GetName(const std::string &table_name, uint64_t id) {
  auto *table = GetTable(table_name);
  for (const auto &record : (*table)) {
    if (record.id == id) {
      return record.name;
    }
  }
  return {};
}