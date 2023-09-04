#ifndef SRC_HOMEWORK_5_VIEW_HPP
#define SRC_HOMEWORK_5_VIEW_HPP

#include "graphical_objects.hpp"
#include <cstdint>
#include <list>
#include <memory>

namespace homework_5 {

class Canvas final {
  std::list<std::unique_ptr<GraphicalObject>> objects_;
  Point begin_point_;
  Point end_point_;

public:
  explicit Canvas(Point begin_point = {}, Point end_point = {-1, -1});
  void AddGraphicalObject(Color color, Point begin_object);
  void Draw();
};

class View final {
  std::list<Canvas> canvases_{Canvas()};

public:
  void AddCanvas();
};

}; // namespace homework_5



#endif /* SRC_HOMEWORK_5_VIEW_HPP */
