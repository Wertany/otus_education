#ifndef SRC_HOMEWORK_2_HOMEWORK_2_HPP
#define SRC_HOMEWORK_2_HOMEWORK_2_HPP

#include <string>
#include <vector>

namespace homework_2 {

class Filter {
private:
  std::vector<std::vector<std::string>> ip_pool_{};
  std::vector<std::string> Split(const std::string &str_, char d);
  void
  ReverseLexicographicOrder(std::vector<std::vector<std::string>> &ip_pool);
  void Sorting(std::vector<std::vector<std::string>> &ip_pool);

public:
  Filter();
  Filter(std::string &content);
  Filter(std::vector<std::vector<std::string>> ip_pool) : ip_pool_(ip_pool){};
  [[nodiscard]] std::vector<std::vector<std::string>> 
  filter(int16_t ip_part_1 = -1, int16_t ip_part_2 = -1, int16_t ip_part_3 = -1);
  [[nodiscard]] std::vector<std::vector<std::string>> filter_any(uint8_t value);
};

} // namespace homework_2

#endif /* SRC_HOMEWORK_2_HOMEWORK_2_HPP */
