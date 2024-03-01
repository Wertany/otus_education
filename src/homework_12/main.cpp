#include "fmt/base.h"
#include "mapreduce.hpp"
#include <filesystem>

int main(int argc, char *argv[]) {

  if (argc != 4) {
    fmt::print("Invalid arguments\n");
    return -1;
  }

  auto *file_path(argv[1]);
  if (!std::filesystem::exists(file_path)) {
    fmt::print("File {} is not exists\n", file_path);
    return -1;
  }

  auto m = static_cast<uint32_t>(std::atoi(argv[2]));
  auto r = static_cast<uint32_t>(std::atoi(argv[3]));

  auto time_point{std::chrono::steady_clock::now()};

  MapReduce mr(m, r, file_path);

  if (!mr.Map()) {
    fmt::print("{}\n", mr.GetError());
    return -1;
  }

  mr.Shuffle();

  if (!mr.Reduce()) {
    fmt::print("{}\n", mr.GetError());
    return -1;
  }

  fmt::print("Minimum prefix is {}\n", mr.GetMinPrefix());
  fmt::print("Total time is {} msec\n",
             std::chrono::duration_cast<std::chrono::milliseconds>(
                 std::chrono::steady_clock::now() - time_point)
                 .count());
  return -1;
}