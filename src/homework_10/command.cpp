#include "command.hpp"

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <utility>

#include "fmt/base.h"
#include "fmt/format.h"

namespace homework_9 {

void CommandAccumulator::AddCommand(std::string_view command) {
  if (commands_.empty()) { first_command_time_ = std::chrono::system_clock::now(); }
  commands_.emplace_back(command);
}

std::pair<std::string, std::string> CommandAccumulator::GetBlock() {
  using fmt::format;
  auto time = format("bulk{}", first_command_time_.time_since_epoch().count());

  std::string bulk_message{"bulk: "};

  for (const auto &command : commands_) { bulk_message += command + ", "; }
  *(bulk_message.rbegin() + 1) = '\n';

  commands_.clear();
  return {time, bulk_message};
}

CommandProcessor::CommandProcessor(std::size_t num_commands)
    : num_commands_(num_commands) {}

void CommandProcessor::AddCommand(std::string_view cmd) {
  std::istringstream iss(cmd.data());
  ProcessCommands(iss);
}

void CommandProcessor::Stop() {
  if (!accumulator_.Empty()) { LogBlock(); }
}

void CommandProcessor::ProcessCommands(std::istream &stream) {
  std::string command{};

  while (stream >> command) {
    if (command == "{") {
      if (num_blocks_ == 0) { LogBlock(); }
      num_blocks_++;
      continue;
    }

    if (command == "}") {
      if (num_blocks_ == SIZE_MAX) {
        std::cerr << "Ошибка в блоках лишний символ }\n";
        num_blocks_ = 0;
        continue;
      }

      num_blocks_--;
      if (num_blocks_ == 0) { LogBlock(); }
      continue;
    }

    accumulator_.AddCommand(command);
    if (num_blocks_ == 0 && accumulator_.Size() == num_commands_) { LogBlock(); }
  }
}

void CommandProcessor::LogBlock() {
  auto block = accumulator_.GetBlock();
  queue_blocks_for_console_.push(block.second);
  queue_blocks_for_files_.push(block);
}

}// namespace homework_9