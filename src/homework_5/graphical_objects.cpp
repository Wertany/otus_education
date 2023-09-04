#include "graphical_objects.hpp"
#include "fmt/core.h"

namespace homework_5 {

GraphicalObject::GraphicalObject(Color color, Point begin_object)
    : color_(color), begin_object_(begin_object) {}

Text::Text(Color color, Point begin_object)
    : GraphicalObject(color, begin_object) {}

void Text::Draw() { fmt::print("Text is {}", data_); }

Line::Line(Color color, Point begin_object) : GraphicalObject(color, begin_object) {}

void Line::Draw() { fmt::print("_\n", line_); }

Square::Square(Color color, Point begin_object)
    : GraphicalObject(color, begin_object) {}

void Square::Draw() {
  fmt::print("-----------------Print Square-----------------\n");
  for (auto line : lines_) {
    line.Draw();
  }
  fmt::print("----------------------------------------------\n");
}

void Square::AddLine(Color color, Point begin_object) {
  lines_.emplace_back(color, begin_object);
}

Rectangle::Rectangle(Color color, Point begin_object)
    : GraphicalObject(color, begin_object) {}

void Rectangle::Draw() {
  fmt::print("-----------------Print Rectangle-----------------\n");
  for (auto line : lines_) {
    line.Draw();
  }
  fmt::print("----------------------------------------------\n");
}

void Rectangle::AddLine(Color color, Point begin_object) {
  lines_.emplace_back(color, begin_object);
}

} // namespace homework_5
