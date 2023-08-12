#ifndef SRC_HOMEWORK_3_VECTOR_HPP
#define SRC_HOMEWORK_3_VECTOR_HPP

#include <cstddef>
#include <memory>
#include <optional>

#include "homework_3/allocator.hpp"

namespace homework_3 {

constexpr std::size_t kDefaultReserve{10};

template <typename T, class Alloc = MyAllocator<T>> class MyVector {
private:
  Alloc allocator_;

  std::size_t size_{0};
  T *data_{nullptr};
  std::size_t capacity_;

public:
  MyVector() = default;
  explicit MyVector(std::size_t size)
      : size_(size), data_{new T[size]}, capacity_(size) {}
  ~MyVector() { allocator_.deallocate(data_, capacity_); }

  T &operator[](std::size_t index) {
    return data_[index];
  }
  const T &operator[](std::size_t index) const {
    return data_[index];
  }

  auto size() const { return size_; }
  auto capacity() const { return capacity_; }

  void reserve(std::size_t capacity) {
    data_ = allocator_.allocate(capacity);

    if (data_) {
      capacity_ = capacity;
    }
  }

  void push_back(const T &value) {
    if (capacity_ == 0) {
      reserve(kDefaultReserve);
    } else if (size_ == capacity_) {
      reserve(capacity_ * 2);
    }
    allocator_.construct(&data_[size_], value);
    size_++;
  }
};

} // namespace homework_3

#endif /* SRC_HOMEWORK_3_VECTOR_HPP */
