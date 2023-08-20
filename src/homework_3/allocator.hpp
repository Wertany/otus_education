#ifndef SRC_HOMEWORK_3_ALLOCATOR_HPP
#define SRC_HOMEWORK_3_ALLOCATOR_HPP

#include <array>
#include <cstddef>
#include <cstdlib>
#include <fmt/format.h>
#include <iostream>
#include <limits>
#include <memory>
#include <new>

namespace homework_3 {

/**
 * @brief Аллокатор памяти, который позволяет выполнить операцию резервирования
 * памяти с помощью.
 */
template <class T, std::size_t Size = 10> class MyAllocator {
public:
  using value_type = T;
  using pointer = value_type *;
  using size_type = std::size_t;

  MyAllocator() noexcept = default;
  ~MyAllocator() = default;

  template <class U>
  constexpr explicit MyAllocator(const MyAllocator<U, Size> &alloc) noexcept {}

  MyAllocator(const MyAllocator<T, Size> &other) = default;

  [[nodiscard]] pointer allocate(std::size_t n) {
    // fmt::print("Allocator allocate\n");

    if (n + last_index_ < max_size_) {
      return memory_ + last_index_ + 1;
    }

    auto p = reinterpret_cast<pointer>(
        std::malloc((n > Size ? n : Size) * sizeof(T)));
    if (!p) {
      throw std::bad_alloc();
    }

    for (size_type index = 0; index < last_index_; ++index) {
      p[index] = memory_[index];
    }

    for (size_type index = 0; index < last_index_; ++index) {
      destroy(&memory_[index]);
    }
    std::free(memory_);
    
    max_size_ = n;
    memory_ = p;

    return memory_;
  }

  void deallocate(pointer p, size_type) {
    // fmt::print("Allocator deallocate\n");
    std::free(p);
  }

  template <class Up, class... Args> void construct(Up *p, Args &&...args) {
    // fmt::print("Allocator construct\n");
    ++last_index_;
    ::new (reinterpret_cast<void *>(p)) Up(std::forward<Args>(args)...);
  }

  template <class U> void destroy(U *p) {
    --last_index_;
    p->~U();
  }

  template <class U> struct rebind { // NOLINT
    using other = MyAllocator<U, Size>;
  };

  using propagate_on_container_copy_assignment = std::true_type;
  using propagate_on_container_move_assignment = std::true_type;
  using propagate_on_container_swap = std::true_type;

private:
  pointer memory_{nullptr};
  size_type last_index_{0};
  size_type max_size_{0};
};

template <class T, class U>
constexpr bool
operator==([[maybe_unused]] const homework_3::MyAllocator<T> &left,
           [[maybe_unused]] const homework_3::MyAllocator<U> &right) noexcept {
  return left.pool == right.pool;
}

template <class T, class U>
constexpr bool
operator!=([[maybe_unused]] const homework_3::MyAllocator<T> &left,
           [[maybe_unused]] const homework_3::MyAllocator<U> &right) noexcept {
  return left.pool != right.pool;
}

} // namespace homework_3

#endif /* SRC_HOMEWORK_3_ALLOCATOR_HPP */
