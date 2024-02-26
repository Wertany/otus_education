#include "async.hpp"

#include <array>
#include <cstddef>
#include <memory>
#include <thread>

#include "command.hpp"
#include "threads_observers.hpp"

namespace homework_9::async {

namespace {
constexpr std::size_t kNumThreads = 3;

struct AsyncHandler {
  explicit AsyncHandler(std::size_t bulk) : processor(bulk) {
    observers_ = {std::make_unique<LoggingToConsole>(processor),
                 std::make_unique<LoggingToFile>(processor),
                 std::make_unique<LoggingToFile>(processor)};
  }
  ~AsyncHandler() {
    processor.Stop();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    for (const auto &observer : observers_) {
      observer->Stop();
    }
  }

  CommandProcessor processor;

 private:
  std::array<std::unique_ptr<Observer>, kNumThreads> observers_;
};
}// namespace

handle_t connect(std::size_t bulk) { return new AsyncHandler(bulk); }

void receive(handle_t handle, const char *data, std::size_t) {
  auto *processor = reinterpret_cast<AsyncHandler *>(handle);
  processor->processor.AddCommand(data);
}

void disconnect(handle_t handle) {
  auto *async_handle = reinterpret_cast<AsyncHandler *>(handle);
  delete async_handle;
}

}// namespace homework_9::async
