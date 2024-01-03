#ifndef SRC_HOMEWORK_6_HOMEWORK_6_HPP
#define SRC_HOMEWORK_6_HOMEWORK_6_HPP

#include <cstddef>
#include <map>
#include <type_traits>

namespace homework_6 {

template<class T, std::enable_if_t<std::is_arithmetic_v<T>, int> DefaultValue = 0>
class Matrix {
 private:
  std::map<std::pair<std::size_t, std::size_t>, T> data_{};
  int default_value_;

 public:
  explicit Matrix() : default_value_(DefaultValue) {}

  Matrix(const Matrix &) = delete;
  Matrix(Matrix &&) = delete;

  T GetValue(std::size_t i, std::size_t j) {
    if (auto need_elem = data_.find({i, j}); need_elem != data_.end()) {
      return need_elem->second;
    }
    return default_value_;
  }

  /**
   * @brief Set the Value object
   * 
   * @param i Индекс по строке
   * @param j Индекс по столбцу
   * @param value Значение, которое необходимое вставить или заменить
   */
  void SetValue(std::size_t i, std::size_t j, T value) {
    data_.insert_or_assign({i, j}, value);
  }

  auto size() { return data_.size(); }

  auto begin() { return data_.begin(); }

  auto end() { return data_.end(); }

  /**
   * @brief Представление значение в матрице.
   * Реализовано для удобства получения значения с помощью [][].
   */
  class Value {
   public:
    Value(std::size_t line, std::size_t column, Matrix &matrix)
        : line_(line),
          column_(column),
          matrix_(matrix) {}

    auto GetValue() { return matrix_.GetValue(line_, column_); }

    template<class Val>
    bool operator==(Val value) {
      return GetValue() == value;
    }

    auto &operator=(T value) {
      matrix_.SetValue(line_, column_, value);
      return *this;
    }

   private:
    std::size_t line_;
    std::size_t column_;
    Matrix &matrix_;
  };

  /**
   * @brief Представление строки в матрице.
   * Реализовано для удобства получения значения с помощью [][].
   */
  class Line {
   public:
    Line(std::size_t line, Matrix &matrix) : line_(line), matrix_(matrix) {}

    auto operator[](std::size_t column) { return Value(line_, column, matrix_); }

   private:
    std::size_t line_;
    Matrix &matrix_;
  };

  Line operator[](std::size_t line) { return Line(line, *this); }
};

}// namespace homework_6

#endif /* SRC_HOMEWORK_6_HOMEWORK_6_HPP */
