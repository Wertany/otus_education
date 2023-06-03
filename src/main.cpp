#include <iostream>

#include "homework_2/homework_2.hpp"

int main() {
  homework_2::Filter filter{};

  try {
    std::cout << "filter()\n";
    [[maybe_unused]] auto temp_var_1 = filter.filter();
    std::cout << "\n\n";
    std::cout << "filter(1)\n";
    [[maybe_unused]] auto temp_var_2 = filter.filter(1);
    std::cout << "\n\n";
    std::cout << "filter(46, 70)\n";
    [[maybe_unused]] auto temp_var_3 = filter.filter(46, 70);
    std::cout << "\n\n";
    std::cout << "filter_any(46)\n";
    [[maybe_unused]] auto temp_var_4 = filter.filter_any(46);
  } catch (const std::exception &e) {
    std::cout << e.what() << std::endl;
  }
}