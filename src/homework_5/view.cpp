#include "view.hpp"

#include <algorithm>
#include <memory>
#include <stdexcept>
#include <string_view>
#include <utility>

#include "fmt/core.h"
#include "graphical_objects.hpp"

namespace homework_5 {

namespace {
std::list<View> views;
}

Canvas::Canvas(std::string_view id, Point begin_point, Point end_point)
    : id_(id),
      begin_point_(begin_point),
      end_point_(end_point) {}

void Canvas::AddGraphicalObject(std::unique_ptr<GraphicalObject> object) {
  objects_.emplace_back(std::move(object));
}

Canvas *View::GetCanvas(std::string_view id_canvas) {
  for (auto &canvas : canvases_) {
    if (canvas.GetId() == id_canvas) { return &canvas; }
  };

  return nullptr;
}

void Canvas::Draw() {
  fmt::print("--------------------Canvas--------------------\n");
  fmt::print("Print canvas begin point: x = {}, y = {}, end point: x = {}, y = {}\n",
             begin_point_.x, begin_point_.y, end_point_.x, end_point_.y);
  for (auto &object : objects_) { object->Draw(); }

  fmt::print("-------------------End Canvas-----------------\n");
}

Json Canvas::Export() {
  /* В данной функции проходимся по всем объектам и вызываем у них экспорт структур json */
  fmt::print("Export settings Canvas");
  for (const auto &object : objects_) { object->Export(); }
  return {};
}

void Canvas::Import(Json &settings) {
  /* Создаём графические объекты, добавляя в контейнер */
}

Canvas &View::AddCanvas(std::string_view id, Point begin_point, Point end_point) {
  auto position = std::find_if(canvases_.begin(), canvases_.end(),
                               [id](Canvas &canvas) { return canvas.GetId() == id; });

  if (position != canvases_.end()) {
    throw std::runtime_error(fmt::format("ID {} in view {} exists", id, id_));
  }

  canvases_.emplace_back(id, begin_point, end_point);
  return canvases_.back();
}

void View::AddGraphicalObject(std::string_view id_canvas,
                              std::unique_ptr<GraphicalObject> object) {
  auto *canvas = GetCanvas(id_canvas);
  if (canvas != nullptr) {
    canvas->AddGraphicalObject(std::move(object));
    return;
  }

  throw std::runtime_error(fmt::format("id canvas {} not exists", id_canvas));
}

void View::Draw() {
  fmt::print("---------------------View---------------------\n");

  for (auto &canvas : canvases_) { canvas.Draw(); }

  fmt::print("--------------------End View------------------\n");
}

std::list<View> &GetViews() { return views; }

View &AddView(std::string_view id) {
  auto position = std::find_if(views.begin(), views.end(),
                               [id](View &view) { return view.GetId() == id; });

  if (position != views.end()) {
    throw std::runtime_error(fmt::format("View with ID {} exists", id));
  }

  views.emplace_back(id);
  return views.back();
}

std::unique_ptr<GraphicalObject> FactoryGraphicalObject::MakeText(int32_t x, int32_t y,
                                                                  Color color,
                                                                  std::string_view data) {
  return std::make_unique<Text>(color, Point{x, y}, data);
}

std::unique_ptr<GraphicalObject> FactoryGraphicalObject::MakeLine(int32_t x, int32_t y,
                                                                  Color color,
                                                                  std::string_view line) {
  return std::make_unique<Line>(color, Point{x, y}, line);
}

std::unique_ptr<GraphicalObject> FactoryGraphicalObject::MakeSquare(int32_t x, int32_t y,
                                                                    Color color,
                                                                    std::size_t rad) {
  return std::make_unique<Square>(color, Point{x, y}, rad);
}

std::unique_ptr<GraphicalObject> FactoryGraphicalObject::MakeRectangle(
    int32_t x, int32_t y, Color color, std::list<Line> lines) {
  auto rectangle = std::make_unique<Rectangle>(color, Point{x, y});

  for (auto &line : lines) { rectangle->AddLine(line); }
  return rectangle;
}

}// namespace homework_5