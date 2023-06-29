#include <algorithm>
#include <filesystem>
#include <fmt/format.h>
#include <fstream>
#include <gtest/gtest.h>
#include <sys/types.h>
#include <vector>

#include "homework_2/homework_2.hpp"

namespace {

constexpr std::string_view kNameFile{"ip_filter.tsv"};
const auto kPathFile = std::filesystem::current_path() / kNameFile;

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

bool CompareIpInDescendingOrder(
    std::vector<std::vector<uint16_t>>::iterator begin,
    std::vector<std::vector<uint16_t>>::iterator end, uint8_t i) {
  return std::is_sorted(
      begin, end, [i](std::vector<uint16_t> &lhs, std::vector<uint16_t> &rhs) {
        return lhs.at(i) > rhs.at(i);
      });
}

auto content = file_contents(kPathFile);

TEST(Homework2, FilterNoneArgument) {
  [[maybe_unused]] homework_2::Filter filter(content);

  auto sorted_ip_pool = filter.filter();

  ASSERT_TRUE(CompareIpInDescendingOrder(sorted_ip_pool.begin(),
                                         sorted_ip_pool.end(), 0));
}

} // namespace