#include <algorithm>
#include <filesystem>
#include <fmt/format.h>
#include <fstream>
#include <gtest/gtest.h>

#include "homework_2/homework_2.hpp"

namespace {

constexpr std::string_view name_file{"ip_filter.tsv"};
const auto path_file = std::filesystem::current_path() / name_file;

[[nodiscard]] std::string file_contents(const std::filesystem::path &path) {
  if (!std::filesystem::is_regular_file(path)) {
    return {};
  }

  std::ifstream file(path, std::ios::in | std::ios::binary);
  if (!file.is_open()) {
    return {};
  }

  std::string content{std::istreambuf_iterator<char>(file),
                      std::istreambuf_iterator<char>()};

  file.close();

  return content;
}

bool CompareIpInDescendingOrder(std::vector<std::vector<std::string>>::iterator begin,
                                std::vector<std::vector<std::string>>::iterator end,
                                uint8_t i) {
  return std::is_sorted(
      begin, end, [i](std::vector<std::string> &lhs, std::vector<std::string> &rhs) {
        return std::atoi(lhs.at(i).c_str()) > std::atoi(rhs.at(i).c_str());
      });
}

auto content = file_contents(path_file);

TEST(Homework_2, filter_none_argument) {
  [[maybe_unused]] homework_2::Filter filter(content);

  auto sorted_ip_pool = filter.filter();

  ASSERT_TRUE(
      CompareIpInDescendingOrder(sorted_ip_pool.begin(), sorted_ip_pool.end(), 0));
}

} // namespace