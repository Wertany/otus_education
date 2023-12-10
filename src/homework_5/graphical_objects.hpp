#ifndef SRC_HOMEWORK_5_GRAPHICAL_OBJECTS_HPP
#define SRC_HOMEWORK_5_GRAPHICAL_OBJECTS_HPP

#include <cstddef>
#include <cstdint>
#include <list>
#include <memory>
#include <string>
#include <string_view>

namespace homework_5 {

struct Color final {
  uint_fast8_t r{0};
  uint_fast8_t g{0};
  uint_fast8_t b{0};
};

struct Point final {
  int32_t x{0};
  int32_t y{0};
};

/**
 * @brief структура json для экспорта графического объекта.
 *
 * Допустим, что данная структура взята из библиотеки nlohmann::json (https://github.com/nlohmann/json).
 * Внедрять библиотеку в данный проект не стал.
 */
struct Json final {};

/**
 * @brief Абстрактный класс, представляющий графический объект
 */
class GraphicalObject {
 protected:
  Color color_;
  Point begin_object_;///< Точка начала объекта

 public:
  GraphicalObject(Color color, Point begin_object);
  virtual ~GraphicalObject() = default;

  auto &GetPoint() { return begin_object_; }

  virtual void Draw() = 0;
  virtual Json Export() = 0;
};

class Text final : public GraphicalObject {
  std::string data_;

 public:
  Text(Color color, Point begin_object, std::string_view data);

  void Draw() final;
  Json Export() final;
};

class Line final : public GraphicalObject {
  std::string line_;

 public:
  Line(Color color, Point begin_object, std::string_view line);

  void Draw() final;
  Json Export() final;
};

class Square final : public GraphicalObject {
  std::size_t rad_;///< Радиус круга

 public:
  Square(Color color, Point begin_object, std::size_t rad);

  void Draw() final;
  Json Export() final;
};

class Rectangle final : public GraphicalObject {
  std::list<std::unique_ptr<Line>> lines_;

 public:
  Rectangle(Color color, Point begin_object);

  void AddLine(Color color, Point begin_object, std::string_view line);
  void AddLine(Line &line);

  void Draw() final;
  Json Export() final;
};

}// namespace homework_5

#endif /* SRC_HOMEWORK_5_GRAPHICAL_OBJECTS_HPP */
