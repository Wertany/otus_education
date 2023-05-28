#include <iostream>

#include "homework_2/homework_2.hpp"

int main() {
  homework_2::Filter filter{};

  filter.filter();
  filter.filter_any(185);
}