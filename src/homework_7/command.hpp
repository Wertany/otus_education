#ifndef SRC_HOMEWORK_7_COMMAND_HPP
#define SRC_HOMEWORK_7_COMMAND_HPP

#include <chrono>
#include <cstddef>
#include <istream>
#include <list>
#include <string>
#include <string_view>

namespace homework_7 {

class CommandAccumulator final {
 public:
  void AddCommand(std::string_view command);

  void LogCommands();
  void LogAndClearCommands();

  auto Size() { return commands_.size(); }

  auto Empty() { return commands_.empty(); }

 private:
  std::list<std::string> commands_;
  std::chrono::system_clock::time_point first_command_time_;
};

class CommandProcessor final {
 public:
  explicit CommandProcessor(std::size_t num_commands);

  void ProcessCommands(std::istream &stream);

 private:
  CommandAccumulator accumulator_;
  std::size_t num_commands_;
};

}// namespace homework_7

#endif /* SRC_HOMEWORK_7_COMMAND_HPP */
