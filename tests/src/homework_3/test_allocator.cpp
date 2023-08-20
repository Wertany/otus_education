#include <gtest/gtest.h>

#include "homework_3/vector.hpp"
#include <fmt/core.h>

namespace {

std::size_t factorial(std::size_t n) { // NOLINT
  return (n == 1 || n == 0) ? 1 : factorial(n - 1) * n;
}

TEST(Homework3, MainTask) {
  try {
    std::map<int32_t, int32_t> classic_map;

    for (uint_fast8_t i = 0; i < 10; i++) {
      classic_map.emplace(i, factorial(i));
    }

    std::map<int32_t, int32_t, std::less<>,
             homework_3::MyAllocator<std::pair<const int32_t, int32_t>>>
        map_with_my_alloc;

    for (uint_fast8_t i = 0; i < 10; i++) {
      map_with_my_alloc.emplace(i, factorial(i));
    }

    for (const auto &[key, value] : map_with_my_alloc) {
      fmt::print("{} {}\n", key, value);
    }

    homework_3::MyVector<int32_t> my_vector;
    for (uint_fast8_t i = 0; i < 10; i++) {
      my_vector.push_back(factorial(i));
    }

    for (uint_fast8_t i = 0; i < 10; i++) {
      fmt::print("{} ", my_vector[i]);
    }
  } catch (const std::runtime_error &e) {
    fmt::print("{}", e.what());
  }
}

} // namespace