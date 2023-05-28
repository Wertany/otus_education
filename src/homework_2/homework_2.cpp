#include "homework_2.hpp"

#include <algorithm>
#include <iostream>

namespace homework_2 {

std::vector<std::string> Filter::Split(const std::string &str_, char d) {
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

void Sort(std::vector<std::vector<std::string>>::iterator begin,
          std::vector<std::vector<std::string>>::iterator end, uint8_t i) {
  std::sort(begin, end,
            [i](std::vector<std::string> &lhs, std::vector<std::string> &rhs) {
              return std::atoi(lhs.at(i).c_str()) >
                     std::atoi(rhs.at(i).c_str());
            });
}

constexpr uint8_t count_part_ip{4};
void Filter::ReverseLexicographicOrder(
    std::vector<std::vector<std::string>> &ip_pool) {
  for (uint8_t i = 0; i < count_part_ip; i++) {
    if (i == 0) {
      Sort(ip_pool.begin(), ip_pool.end(), i);
      continue;
    }

    for (std::vector<std::vector<std::string>>::iterator begin =
             ip_pool.begin();
         begin + 1 != ip_pool.cend();) {
      for (auto end = begin + 1; end != ip_pool.cend(); end++) {
        uint8_t temp = i - 1;
        if (begin->at(temp).compare(end->at(temp)) != 0) {
          if (begin != end - 1) {
            Sort(begin, end, i);
          }

          begin = end;
          break;
        }

        if (end + 1 == ip_pool.cend() && begin != ip_pool.end() - 1) {
          Sort(begin, ip_pool.end(), i);
          begin = end;
          break;
        }
      }
    }
  }
}

void PrintIP(const std::vector<std::string> &ip) {
  for (const auto &ip_part : ip) {
    if (ip_part != *ip.cbegin()) {
      std::cout << ".";
    }
    std::cout << ip_part;
  }
  std::cout << std::endl;
}

void PrintListIP(const std::vector<std::vector<std::string>> &list_ip) {
  for (const auto &ip : list_ip) {
    PrintIP(ip);
  }
}

void Filter::Sorting(std::vector<std::vector<std::string>> &ip_pool) {
  try {
    ReverseLexicographicOrder(ip_pool);
    PrintListIP(ip_pool);
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
}

Filter::Filter() {
  for (std::string line; std::getline(std::cin, line);) {
    if (line.empty()) {
      break;
    }
    std::vector<std::string> whole_line = Split(line, '\t');
    ip_pool_.push_back(Split(whole_line.at(0), '.'));
  }
}

std::vector<std::vector<std::string>>
Filter::filter(int16_t ip_part_1, int16_t ip_part_2, int16_t ip_part_3) {
  if (ip_part_1 > 0 && ip_part_1 <= 255) {
    std::vector<std::vector<std::string>> temp_ip_vector{};
    if (ip_part_2 > 0 && ip_part_2 <= 255) {
      if (ip_part_3 > 0 && ip_part_3 <= 255) {
        auto search = [ip_part_1, ip_part_2,
                       ip_part_3](const std::vector<std::string> &ip) -> bool {
          if (std::atoi(ip[0].c_str()) == ip_part_1 &&
              std::atoi(ip[1].c_str()) == ip_part_2 &&
              std::atoi(ip[2].c_str()) == ip_part_3) {
            return true;
          }
          return false;
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
                     ip_part_2](const std::vector<std::string> &ip) -> bool {
        if (std::atoi(ip[0].c_str()) == ip_part_1 &&
            std::atoi(ip[1].c_str()) == ip_part_2) {
          return true;
        }
        return false;
      };

      for (auto const &ip : ip_pool_) {
        if (search(ip)) {
          temp_ip_vector.push_back(ip);
        }
      }
      Sorting(temp_ip_vector);

      return temp_ip_vector;
    }

    auto search = [ip_part_1](const std::vector<std::string> &ip) -> bool {
      if (std::atoi(ip[0].c_str()) == ip_part_1) {
        return true;
      }
      return false;
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

std::vector<std::vector<std::string>> Filter::filter_any(uint8_t value) {
  std::vector<std::vector<std::string>> temp_ip_vector{};

  for (const auto &ip : ip_pool_) {
    for (const auto &ip_part : ip) {
      if (std::atoi(ip_part.c_str()) == value) {
        temp_ip_vector.push_back(ip);
        break;
      }
    }
  }

  Sorting(temp_ip_vector);

  return temp_ip_vector;
}

} // namespace homework_2
