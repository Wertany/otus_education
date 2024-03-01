#include "accuracy_finder.hpp"
#include "fmt/base.h"
#include <string>

int main(int argc, char *argv[]) {
  std::string name_file;
  if (argc != 2) {
    name_file = "test.csv";
  } else {
    name_file = argv[1];
  }
  AccuracyFinder finder;
  fmt::print("{}\n", finder.GetAccuracy(name_file));
}