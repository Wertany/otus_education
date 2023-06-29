#ifndef SRC_HOMEWORK_2_HOMEWORK_2_HPP
#define SRC_HOMEWORK_2_HOMEWORK_2_HPP

#include <string>
#include <utility>
#include <vector>

namespace homework_2 {

class Filter {
private:
  std::vector<std::vector<uint16_t>> ip_pool_{};

public:
  Filter();
  explicit Filter(const std::string &content);
  [[nodiscard]] std::vector<std::vector<uint16_t>>
  filter(int16_t ip_part_1 = -1, int16_t ip_part_2 = -1,
         int16_t ip_part_3 = -1);
  [[nodiscard]] std::vector<std::vector<uint16_t>> filter_any(uint8_t value);
};

} // namespace homework_2

#endif /* SRC_HOMEWORK_2_HOMEWORK_2_HPP */
