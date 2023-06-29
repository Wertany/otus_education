#include "homework_2.hpp"

#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <iostream>

namespace homework_2 {

namespace {

std::vector<std::string> Split(const std::string &str_, char d) {
  std::vector<std::string> str{};

  std::string::size_type start{};
  std::string::size_type stop = str_.find_first_of(d);
  while (stop != std::string::npos) {
    str.push_back(str_.substr(start, stop - start));

    start = stop + 1;
    stop = str_.find_first_of(d, start);
  }

  stop = str_.find_first_of(' ', start);
  str.push_back(str_.substr(start, stop - start));

  return str;
}

std::vector<uint16_t> StringToUint(std::vector<std::string> &&getting_ip) {
  std::vector<uint16_t> ip{};

  ip.reserve(getting_ip.size());
  for (auto &part_ip : getting_ip) {
    ip.push_back(std::atoi(part_ip.c_str()));
  }

  return ip;
}

void PrintListIP(const std::vector<std::vector<uint16_t>> &list_ip) {
  for (const auto &ip : list_ip) {
    for (auto ip_part = ip.cbegin(); ip_part != ip.cend(); ++ip_part) {
      if (ip_part != ip.cbegin()) {
        std::cout << ".";
      }
      std::cout << *ip_part;
    }
    std::cout << std::endl;
  }
}

void Sort(std::vector<std::vector<uint16_t>>::iterator begin,
          std::vector<std::vector<uint16_t>>::iterator end, uint8_t i) {
  std::sort(begin, end,
            [i](std::vector<uint16_t> &lhs, std::vector<uint16_t> &rhs) {
              return lhs.at(i) > rhs.at(i);
            });
}

// NOLINTBEGIN
void RecurseSortBlocksIP(std::vector<std::vector<uint16_t>>::iterator begin,
                         std::vector<std::vector<uint16_t>>::iterator end,
                         uint8_t index) {
  if (begin == end || begin == end - 1) {
    return;
  }

  Sort(begin, end, index);
  if (index == 3) {
    return;
  }

  for (auto begin_temp = begin; begin_temp + 1 != end;) {
    for (auto end_temp = begin_temp + 1;; end_temp++) {
      if (begin_temp->at(index) != end_temp->at(index)) {
        RecurseSortBlocksIP(begin_temp, end_temp, index + 1);
        begin_temp = end_temp;

        break;
      }

      if (end_temp + 1 == end && begin_temp != end - 1) {
        RecurseSortBlocksIP(begin_temp, end, index + 1);
        begin_temp = end_temp;
        break;
      }
    }
  }
}
// NOLINTEND

void ReverseLexicographicOrder(std::vector<std::vector<uint16_t>> &ip_pool) {
  RecurseSortBlocksIP(ip_pool.begin(), ip_pool.end(), 0);
}

void Sorting(std::vector<std::vector<uint16_t>> &ip_pool) {
  try {
    ReverseLexicographicOrder(ip_pool);
    PrintListIP(ip_pool);
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
}

} // namespace

Filter::Filter() {
  for (std::string line; std::getline(std::cin, line);) {
    if (line.empty()) {
      break;
    }
    auto whole_line = Split(line, '\t');
    ip_pool_.push_back(StringToUint(Split(whole_line.at(0), '.')));
  }
}

Filter::Filter(const std::string &content) {
  std::string::size_type start{};
  std::string::size_type stop = content.find_first_of('\n');
  while (stop != std::string::npos) {
    std::string line = content.substr(start, stop - start);
    auto whole_line = Split(line, '\t');
    ip_pool_.push_back(StringToUint(Split(whole_line.at(0), '.')));

    start = stop + 1;
    stop = content.find_first_of('\n', start);
  }
}

std::vector<std::vector<uint16_t>>
Filter::filter(int16_t ip_part_1, int16_t ip_part_2, int16_t ip_part_3) {
  if (ip_part_1 > 0 && ip_part_1 <= 255) {
    std::vector<std::vector<uint16_t>> temp_ip_vector{};
    if (ip_part_2 > 0 && ip_part_2 <= 255) {
      if (ip_part_3 > 0 && ip_part_3 <= 255) {
        auto search = [ip_part_1, ip_part_2,
                       ip_part_3](const std::vector<uint16_t> &ip) -> bool {
          return ip[0] == ip_part_1 && ip[1] == ip_part_2 && ip[2] == ip_part_3;
        };

        for (auto const &ip : ip_pool_) {
          if (search(ip)) {
            temp_ip_vector.push_back(ip);
          }
        }
        Sorting(temp_ip_vector);

        return temp_ip_vector;
      }

      auto search = [ip_part_1,
                     ip_part_2](const std::vector<uint16_t> &ip) -> bool {
        return ip[0] == ip_part_1 && ip[1] == ip_part_2;
      };

      for (auto const &ip : ip_pool_) {
        if (search(ip)) {
          temp_ip_vector.push_back(ip);
        }
      }
      Sorting(temp_ip_vector);

      return temp_ip_vector;
    }

    auto search = [ip_part_1](const std::vector<uint16_t> &ip) -> bool {
      return ip[0] == ip_part_1;
    };

    for (auto const &ip : ip_pool_) {
      if (search(ip)) {
        temp_ip_vector.push_back(ip);
      }
    }
    Sorting(temp_ip_vector);

    return temp_ip_vector;
  }

  Sorting(ip_pool_);

  return ip_pool_;
}

std::vector<std::vector<uint16_t>> Filter::filter_any(uint8_t value) {
  std::vector<std::vector<uint16_t>> temp_ip_vector{};

  for (const auto &ip : ip_pool_) {
    for (const auto &ip_part : ip) {
      if (ip_part == value) {
        temp_ip_vector.push_back(ip);
        break;
      }
    }
  }

  Sorting(temp_ip_vector);

  return temp_ip_vector;
}

} // namespace homework_2
