#include "scanner.hpp"

#include <fmt/ranges.h>

#include <boost/crc.hpp>
#include <boost/filesystem/directory.hpp>
#include <boost/filesystem/file_status.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/regex/v4/regex_match.hpp>
#include <boost/uuid/detail/md5.hpp>
#include <cstddef>
#include <fstream>
#include <string>
#include <string_view>
#include <vector>

#include "cmd_config.hpp"
#include "fmt/base.h"

namespace homework_8 {

bool Scanner::ScanDuplicates() {
  for (const auto &dir : config_.dirs) {
    if (auto is_excluded_dir =
            std::find(config_.excluded_dirs.cbegin(), config_.excluded_dirs.cend(), dir);
        is_excluded_dir == config_.excluded_dirs.cend()) {
      ScanDir(dir);
    }
  }

  return false;
}

void Scanner::PrintDuplicates() {
  for (const auto &group : duplicates_) {
    std::cout << group.first << " ";
    for (const auto &name : group.second) { fmt::print("{} ", name); }
    std::cout << '\n';
  }
}

void Scanner::ScanDir(std::string_view path) {
  namespace fs = boost::filesystem;
  for (const auto &dir : fs::directory_iterator(path.data())) {
    if (dir.status().type() == fs::regular_file) {
      if (fs::file_size(dir) < config_.min_size_file) { return; }
      auto file = dir.path().string();
      if (!CheckMask(file)) { return; }
      files_for_scan_.push_back(file);
    }
  }

  CheckDuplicate(files_for_scan_);
}

bool Scanner::CheckMask(std::string_view file) {
  return config_.file_masks.empty();

  for (const auto &ch : config_.file_masks) {
    if (boost::regex_match(file.data(), ch)) { break; }
  }

  return false;
}

void Scanner::CheckDuplicate(std::list<std::string> &files_for_scan) {// NOLINT
  if (files_for_scan.size() <= 1) { return; }

  auto lhs = files_for_scan_.back();
  files_for_scan_.pop_back();
  for (auto rhs_it = files_for_scan_.cbegin(); rhs_it != files_for_scan_.cend();) {
    if (FilesAreEqual(lhs, *rhs_it)) {
      fmt::print("{}\n", *rhs_it);
      duplicates_[lhs].push_back(*rhs_it);
      rhs_it = files_for_scan.erase(rhs_it);
    } else {
      rhs_it++;
    }
  }
  CheckDuplicate(files_for_scan);
}

bool Scanner::FilesAreEqual(std::string_view lhs, std::string_view rhs) const {
  std::ifstream first_file(lhs.data(), std::ios::in);
  std::ifstream second_file(rhs.data(), std::ios::in);

  if (!first_file.is_open() || !second_file.is_open()) { return false; }

  for (std::size_t step = 0; first_file.seekg(step) && second_file.seekg(step);
       step += config_.size_block) {
    std::string first_block(config_.size_block, '\0');
    std::string second_block(config_.size_block, '\0');
    first_file.read(first_block.data(), config_.size_block);
    second_file.read(second_block.data(), config_.size_block);
    if (Hash(first_block) != Hash(second_block)) { return false; }
  }

  return true;
}

namespace {
std::size_t CalculateCRC32(std::string_view data) {
  boost::crc_32_type hash;
  hash.process_bytes(data.data(), data.size());
  return hash.checksum();
}

std::size_t CalculateMD5(std::string_view data) {
  boost::uuids::detail::md5 hash;
  boost::uuids::detail::md5::digest_type digest;
  hash.process_bytes(data.data(), data.size());
  hash.get_digest(digest);

  std::size_t result{};
  for (const auto &elem : digest) { result += elem; }

  return result;
}
}// namespace

std::size_t Scanner::Hash(std::string_view data) const {
  return config_.hash_type == HashType::kCRC32 ? CalculateCRC32(data)
                                               : CalculateMD5(data);
}

}// namespace homework_8