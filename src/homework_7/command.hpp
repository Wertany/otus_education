#ifndef SRC_HOMEWORK_7_COMMAND_HPP
#define SRC_HOMEWORK_7_COMMAND_HPP

#include <chrono>
#include <cstddef>
#include <istream>
#include <list>
#include <memory>
#include <string>
#include <string_view>

namespace homework_7 {

class PublicationObserver {
 public:
  virtual void Publish(std::string_view message) = 0;
};

class LoggingToFile final : public PublicationObserver {
 public:
  void Publish(std::string_view message) final;
};

class LoggingToConsole final : public PublicationObserver {
 public:
  void Publish(std::string_view message) final;
};

class CommandAccumulator final {
 public:
  CommandAccumulator();

  void AddCommand(std::string_view command);

  void LogCommands();
  void LogAndClearCommands();

  auto Size() { return commands_.size(); }

  auto Empty() { return commands_.empty(); }

 private:
  std::list<std::string> commands_;
  std::chrono::system_clock::time_point first_command_time_;
  std::list<std::unique_ptr<PublicationObserver>> loggers_;

  void AddObjectLogToFile();
  void AddObjectLogToConsole();
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
