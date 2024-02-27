#ifndef SRC_HOMEWORK_9_BOUNDEDBUFFER_HPP
#define SRC_HOMEWORK_9_BOUNDEDBUFFER_HPP

#include <array>
#include <cstddef>
#include <mutex>
#include <optional>
#include <semaphore>

template<class T, std::size_t N>
class BoundedBuffer {
  std::array<T, N> buf_;
  std::size_t read_pos_{};
  std::size_t write_pos_{};
  std::mutex m_;
  std::counting_semaphore<N> n_empty_slots_{N};
  std::counting_semaphore<N> n_full_slots_{0};

  void do_push(auto&& item) {
    n_empty_slots_.acquire();
    try {
      auto lock = std::unique_lock{m_};
      buf_[write_pos_] = std::forward<decltype(item)>(item);
      write_pos_ = (write_pos_ + 1) % N;
    } catch (...) {
      n_empty_slots_.release();
      throw;
    }
    n_full_slots_.release();
  }

 public:
  void push(const T& item) { do_push(item); }

  void push(T&& item) { do_push(std::move(item)); }

  auto pop() {
    n_full_slots_.acquire();
    auto item = std::optional<T>{};
    try {
      auto lock = std::unique_lock{m_};
      item = std::move(buf_[read_pos_]);
      read_pos_ = (read_pos_ + 1) % N;
    } catch (...) {
      n_full_slots_.release();
      throw;
    }
    n_empty_slots_.release();
    return std::move(*item);
  }

  auto empty() {
    const auto lock = std::lock_guard{m_};
    return write_pos_ == read_pos_;
  }
};

#endif /* SRC_HOMEWORK_9_BOUNDEDBUFFER_HPP */
