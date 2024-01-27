#ifndef SRC_HOMEWORK_8_FILES_HPP
#define SRC_HOMEWORK_8_FILES_HPP

#include <map>
#include <string_view>
#include <vector>

#include "cmd_config.hpp"

namespace homework_8 {

class Scanner {
 public:
  explicit Scanner(CMDConfig &config) : config_(config) {}

  bool ScanDuplicates();
  void PrintDuplicates();

 private:
  CMDConfig &config_;
  std::list<std::string> files_for_scan_;
  std::map<std::string, std::vector<std::string>> duplicates_;

  void ScanDir(std::string_view path);
  bool CheckMask(std::string_view file);
  void CheckDuplicate(std::list<std::string> &files_for_scan);
  [[nodiscard]] bool FilesAreEqual(std::string_view lhs, std::string_view rhs) const;
  [[nodiscard]] std::size_t Hash(std::string_view data) const;
};

}// namespace homework_8

#endif /* SRC_HOMEWORK_8_FILES_HPP */