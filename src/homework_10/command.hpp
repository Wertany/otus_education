#ifndef SRC_HOMEWORK_9_COMMAND_HPP
#define SRC_HOMEWORK_9_COMMAND_HPP

#include <chrono>
#include <cstddef>
#include <istream>
#include <list>
#include <string>
#include <string_view>

#include "BoundedBuffer.hpp"
#include "LockFreeQueue.h"

namespace homework_9 {

class CommandAccumulator final {
 public:
  void AddCommand(std::string_view command);

  std::pair<std::string, std::string> GetBlock();

  auto Size() { return commands_.size(); }

  auto Empty() { return commands_.empty(); }

 private:
  std::list<std::string> commands_;
  std::chrono::system_clock::time_point first_command_time_;
};

class CommandProcessor final {
 public:
  explicit CommandProcessor(std::size_t num_commands);

  void AddCommand(std::string_view cmd);

  void Stop();

  [[nodiscard]] auto QueueBlocksFilesIsEmpty() const {
    return queue_blocks_for_files_.empty();
  }

  [[nodiscard]] auto QueueBlocksConsoleIsEmpty() const {
    return queue_blocks_for_console_.empty();
  }

  auto GetFirstBlockFiles() const { return queue_blocks_for_files_.pop(); }

  auto GetFirstBlockConsole() const { return queue_blocks_for_console_.pop(); }

 private:
  void ProcessCommands(std::istream &stream);
  void LogBlock();

  CommandAccumulator accumulator_;
  std::size_t num_commands_{};
  std::size_t num_blocks_{};

  mutable BoundedBuffer<std::pair<std::string, std::string>, 100> queue_blocks_for_files_;
  mutable LockFreeQueue<std::string, 100> queue_blocks_for_console_;
};


}// namespace homework_9

#endif /* SRC_HOMEWORK_9_COMMAND_HPP */
