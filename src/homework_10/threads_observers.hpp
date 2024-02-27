#ifndef SRC_HOMEWORK_9_THREADS_OBSERVERS_HPP
#define SRC_HOMEWORK_9_THREADS_OBSERVERS_HPP

#include <string_view>
#include <thread>

#include "command.hpp"

namespace homework_9 {

class Observer {
 public:
  explicit Observer(CommandProcessor &processor);

  void Stop();
  virtual void Update(std::string_view, std::string_view) = 0;

 protected:
  std::jthread thread_;
  CommandProcessor &processor_;
};

class LoggingToConsole final : public Observer {
 public:
  explicit LoggingToConsole(CommandProcessor &processor);

  void Update(std::string_view, std::string_view) final;
};

class LoggingToFile final : public Observer {
 public:
  explicit LoggingToFile(CommandProcessor &processor);

  void Update(std::string_view, std::string_view) final;
};

}// namespace homework_9

#endif /* SRC_HOMEWORK_9_THREADS_OBSERVERS_HPP */
