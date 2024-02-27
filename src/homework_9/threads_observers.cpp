#include "threads_observers.hpp"

#include <chrono>
#include <fstream>
#include <stop_token>
#include <thread>

#include "command.hpp"
#include "fmt/base.h"
#include "fmt/format.h"

namespace homework_9 {
Observer::Observer(CommandProcessor &processor) : processor_(processor) {}

void Observer::Stop() {
  thread_.request_stop();
  std::this_thread::sleep_for(std::chrono::milliseconds(20));
  if (thread_.joinable()) { thread_.detach(); }
}

LoggingToConsole::LoggingToConsole(CommandProcessor &processor) : Observer(processor) {
  thread_ = std::jthread{[&](std::stop_token stoken) {// NOLINT
    while (!stoken.stop_requested()) {
      if (processor_.QueueBlocksConsoleIsEmpty()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        continue;
      }
      Update(processor.GetFirstBlockConsole(), {});
    }
  }};
}

void LoggingToConsole::Update(std::string_view, std::string_view block) {
  fmt::print("{}", block);
}

LoggingToFile::LoggingToFile(CommandProcessor &processor) : Observer(processor) {
  thread_ = std::jthread{[&](std::stop_token stoken) {// NOLINT
    while (!stoken.stop_requested()) {
      if (processor_.QueueBlocksFilesIsEmpty()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        continue;
      }
      auto block = processor_.GetFirstBlockFiles();
      Update(block.first, block.second);
    }
  }};
}

void LoggingToFile::Update(std::string_view time, std::string_view block) {
  std::stringstream ss;
  ss << std::this_thread::get_id();
  auto filename = fmt::format("{}_{}.log", time, ss.str());

  std::ofstream ofs(filename, std::fstream::in | std::fstream::out | std::fstream::trunc);

  if (ofs.is_open()) {
    ofs << block;
    ofs.close();
  }
}

}// namespace homework_9