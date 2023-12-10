#include "graphical_objects.hpp"

#include <memory>
#include <string_view>

#include "fmt/core.h"

namespace homework_5 {

GraphicalObject::GraphicalObject(Color color, Point begin_object)
    : color_(color),
      begin_object_(begin_object) {}

Text::Text(Color color, Point begin_object, std::string_view data)
    : GraphicalObject(color, begin_object),
      data_(data) {}

void Text::Draw() { fmt::print("Text is {}\n", data_); }

Json Text::Export() {
  fmt::print("Export settings text");
  return {};
}

Line::Line(Color color, Point begin_object, std::string_view line)
    : GraphicalObject(color, begin_object),
      line_(line) {}

void Line::Draw() {
  fmt::print("Print line: {} x = {}, y = {}\n", line_, begin_object_.x, begin_object_.y);
}

Json Line::Export() {
  fmt::print("Export settings line");
  return {};
}

Square::Square(Color color, Point begin_object, std::size_t rad)
    : GraphicalObject(color, begin_object),
      rad_(rad) {}

void Square::Draw() {
  fmt::print("-----------------Print Square-----------------\n");
  fmt::print("x = {}, y = {}, rad = {}\n", begin_object_.x, begin_object_.y, rad_);
  fmt::print("----------------------------------------------\n");
}

Json Square::Export() {
  fmt::print("Export settings square");
  return {};
}

Rectangle::Rectangle(Color color, Point begin_object)
    : GraphicalObject(color, begin_object) {}

void Rectangle::Draw() {
  fmt::print("-----------------Print Rectangle-----------------\n");
  fmt::print("Rectangle begin point x = {}, y = {}\n", begin_object_.x, begin_object_.y);
  for (auto &line : lines_) { line->Draw(); }
  fmt::print("-------------------End Rectangle-----------------\n");
}

Json Rectangle::Export() {
  fmt::print("Export settings rectangle");
  return {};
}

void Rectangle::AddLine(Color color, Point begin_object, std::string_view line) {
  lines_.push_back(std::make_unique<Line>(color, begin_object, line));
}

void Rectangle::AddLine(Line &line) { lines_.push_back(std::make_unique<Line>(line)); }

}// namespace homework_5
