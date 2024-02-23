#include "command.hpp"

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <string_view>

#include "fmt/base.h"
#include "fmt/core.h"
#include "fmt/format.h"
#include "logger.hpp"

namespace homework_7 {

void LoggingToFile::Publish(std::string_view message) {
  Logger::GetInstance().WriteToLogFile(message);
}

void LoggingToConsole::Publish(std::string_view message) { fmt::print(message); }

CommandAccumulator::CommandAccumulator() {
#ifdef LOG_TO_FILE
  AddObjectLogToFile();
#endif// LOG_TO_FILE

#ifdef LOG_TO_CONSOLE
  AddObjectLogToConsole();
#endif// LOG_TO_CONSOLE
}

void CommandAccumulator::AddCommand(std::string_view command) {
  if (commands_.empty()) { first_command_time_ = std::chrono::system_clock::now(); }
  commands_.emplace_back(command);
}

void CommandAccumulator::LogCommands() {
  using fmt::format;
  if (commands_.empty()) { return; }
  Logger::GetInstance().CreateFile(
      format("bulk{}.log", first_command_time_.time_since_epoch().count()));

  std::string bulk_message{"bulk: "};

  for (const auto &command : commands_) { bulk_message += command + ", "; }
  *(bulk_message.rbegin() + 1) = '\n';

  for (const auto &logger : loggers_) { logger->Publish(bulk_message); }
}

void CommandAccumulator::LogAndClearCommands() {
  LogCommands();
  commands_.clear();
}

void CommandAccumulator::AddObjectLogToFile() {
  loggers_.push_back(std::make_unique<LoggingToFile>());
}

void CommandAccumulator::AddObjectLogToConsole() {
  loggers_.push_back(std::make_unique<LoggingToConsole>());
}

CommandProcessor::CommandProcessor(std::size_t num_commands)
    : num_commands_(num_commands) {}

void CommandProcessor::ProcessCommands(std::istream &stream) {
  std::string command{};
  std::size_t num_blocks{};

  while (stream >> command) {
    if (command == "{") {
      if (num_blocks == 0) { accumulator_.LogAndClearCommands(); }
      num_blocks++;
      continue;
    }

    if (command == "}") {
      if (num_blocks == SIZE_MAX) {
        fmt::print("Ошибка в блоках лишний символ }");
        num_blocks = 0;
        continue;
      }

      num_blocks--;
      if (num_blocks == 0) { accumulator_.LogAndClearCommands(); }
      continue;
    }

    accumulator_.AddCommand(command);
    if (num_blocks == 0 && accumulator_.Size() == num_commands_) {
      accumulator_.LogAndClearCommands();
    }
  }

  if (!accumulator_.Empty() && num_blocks == 0) { accumulator_.LogAndClearCommands(); }
}

}// namespace homework_7