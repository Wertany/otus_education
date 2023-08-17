#include <gtest/gtest.h>
#include <string>

#include "homework_4/print_ip.hpp"

namespace {

TEST(Homework4, MainTask) {
  homework_4::print_ip(int8_t{-1});
  homework_4::print_ip(int16_t{0});
  homework_4::print_ip(int32_t{2130706433});
  homework_4::print_ip(int64_t{8875824491850138409});
  homework_4::print_ip(std::string("Hello, World!"));
  homework_4::print_ip(std::vector<int>{100, 200, 300, 400}); // 100.200.300.400
  homework_4::print_ip(std::list<int16_t>{400, 300, 200, 100});
}

} // namespace