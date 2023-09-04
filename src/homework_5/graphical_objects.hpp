#ifndef SRC_HOMEWORK_5_GRAPHICAL_OBJECTS_HPP
#define SRC_HOMEWORK_5_GRAPHICAL_OBJECTS_HPP

#include <cstdint>
#include <list>
#include <string>

namespace homework_5 {

struct Color {
  uint_fast8_t r{0};
  uint_fast8_t g{0};
  uint_fast8_t b{0};
};

struct Point {
  int32_t x_{0};
  int32_t y_{0};
};

class GraphicalObject {
  Color color_;
  Point begin_object_;

public:
  GraphicalObject(Color color, Point begin_object);
  virtual ~GraphicalObject() = default;

  virtual void Draw() = 0;
};

class Text final : public GraphicalObject {
  std::string data_;

public:
  Text(Color color, Point begin_object);

  void Draw() final;
};

class Line final : public GraphicalObject {
  std::string line_{"_"};

public:
  Line(Color color, Point begin_object);

  void Draw() final;
};

class Square final : public GraphicalObject {
  std::list<Line> lines_;

public:
  Square(Color color, Point begin_object);

  void Draw() final;
  void AddLine(Color color, Point begin_object);
};

class Rectangle final : public GraphicalObject {
  std::list<Line> lines_;

public:
  Rectangle(Color color, Point begin_object);

  void Draw() final;
  void AddLine(Color color, Point begin_object);
};

} // namespace homework_5

#endif /* SRC_HOMEWORK_5_GRAPHICAL_OBJECTS_HPP */
