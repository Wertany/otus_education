#include "fmt/core.h"
#include "view.hpp"

int main() {
  try {
    auto &view_main = homework_5::AddView("Main");
    auto &canvas_main =
        view_main.AddCanvas("Main", homework_5::Point{}, homework_5::Point{-1, -1});

    canvas_main.AddGraphicalObject(
        homework_5::FactoryGraphicalObject::MakeText(0, 0, homework_5::Color{}, "test"));


    std::list<homework_5::Line> lines_rectangle;
    lines_rectangle.emplace_back(homework_5::Color{}, homework_5::Point{}, "_");
    lines_rectangle.emplace_back(homework_5::Color{}, homework_5::Point{}, "__");
    lines_rectangle.emplace_back(homework_5::Color{}, homework_5::Point{}, "___");
    auto rectangle = homework_5::FactoryGraphicalObject::MakeRectangle(
        0, 0, homework_5::Color{}, lines_rectangle);
    canvas_main.AddGraphicalObject(std::move(rectangle));


    auto &views = homework_5::GetViews();
    for (auto &view : views) { view.Draw(); }
  } catch (const std::exception &e) { fmt::print("{}", e.what()); }
}