#ifndef SRC_HOMEWORK_5_VIEW_HPP
#define SRC_HOMEWORK_5_VIEW_HPP

#include <cstdint>
#include <list>
#include <memory>
#include <string>
#include <string_view>

#include "graphical_objects.hpp"

namespace homework_5 {

/**
 * @brief Класс холста, который хранит в себе графические объекты
 */
class Canvas final {
 public:
  explicit Canvas(std::string_view id, Point begin_point = {},
                  Point end_point = {-1, -1});

  std::string_view GetId() { return id_; }

  void AddGraphicalObject(std::unique_ptr<GraphicalObject> object);
  void Draw();
  Json Export();
  void Import(Json &settings);

 private:
  std::list<std::unique_ptr<GraphicalObject>> objects_;
  Point begin_point_;///< Точка начала отрисовки холста
  Point end_point_;  ///< Точка конца отрисовки холста
  std::string id_;
};

/**
 * @brief Класс вида, который хранит в себе холсты
 */
class View final {
  std::list<Canvas> canvases_;
  std::string id_;

  Canvas *GetCanvas(std::string_view id_canvas);

 public:
  explicit View(std::string_view id) : id_(id) {}

  Canvas &AddCanvas(std::string_view id, Point begin_point, Point end_point);
  void AddGraphicalObject(std::string_view id_canvas,
                          std::unique_ptr<GraphicalObject> object);

  std::string_view GetId() { return id_; }

  void Draw();
};

class FactoryGraphicalObject {
 public:
  static std::unique_ptr<GraphicalObject> MakeText(int32_t x, int32_t y, Color color,
                                                   std::string_view data);
  static std::unique_ptr<GraphicalObject> MakeLine(int32_t x, int32_t y, Color color,
                                                   std::string_view line);
  static std::unique_ptr<GraphicalObject> MakeSquare(int32_t x, int32_t y, Color color,
                                                     std::size_t rad);
  static std::unique_ptr<GraphicalObject> MakeRectangle(
      int32_t x, int32_t y, Color color, std::list<Line> lines);
};

std::list<View> &GetViews();
View &AddView(std::string_view id);

};// namespace homework_5


#endif /* SRC_HOMEWORK_5_VIEW_HPP */
