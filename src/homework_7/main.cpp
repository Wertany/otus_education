#include <cstddef>
#include <iostream>
#include <string>
#include "command.hpp"

int main(int argc, char *argv[]) {
  std::size_t num_commands{};

  if (argc != 2) {
    std::cerr << "Неверное количество аргументов, на входе в программу\n";
  }

  num_commands = std::stoi(argv[1]);

  homework_7::CommandProcessor processor(num_commands);
  processor.ProcessCommands(std::cin);
}
