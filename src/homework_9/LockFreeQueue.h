#ifndef LOCKFREEQUEUE_H_
#define LOCKFREEQUEUE_H_

#include <array>
#include <atomic>
#include <optional>
#include <utility>

template<class T, size_t N>
class LockFreeQueue {
  std::array<T, N> buffer_{};
  std::atomic<size_t> size_{0};
  size_t read_pos_{};
  size_t write_pos_{};
  static_assert(std::atomic<size_t>::is_always_lock_free);

  void do_push(auto&& item) {
    if (size_.load() == N) { return; }
    buffer_[write_pos_] = std::forward<decltype(item)>(item);
    write_pos_ = (write_pos_ + 1) % N;
    size_.fetch_add(1);
  }

 public:
  void push(T&& item) { do_push(std::move(item)); }

  void push(const T& item) { do_push(item); }

  [[nodiscard]] auto pop() {
    auto val = std::optional<T>{};
    if (size_.load() > 0) {
      val = std::move(buffer_[read_pos_]);
      read_pos_ = (read_pos_ + 1) % N;
      size_.fetch_sub(1);
    }
    return std::move(*val);
  }

  [[nodiscard]] bool empty() const { return size_.load() == 0; }
};

#endif /* LOCKFREEQUEUE_H_ */
