#ifndef SRC_HOMEWORK_5_GRAPHICAL_DOCUMENT_HPP
#define SRC_HOMEWORK_5_GRAPHICAL_DOCUMENT_HPP

#include <filesystem>
#include <utility>

class GraphicalDocument {
 public:
  explicit GraphicalDocument(std::filesystem::path path) : path_(std::move(path)) {}

  /**
   * @brief Вычитываем настройки видов и т.п. c файла по пути path_
   */
  void Import();
  /**
   * @brief Сохраняем настройки видов и т.д. в файл по пути path_
   * 
   */
  void Export();
 private:
  std::filesystem::path path_;
};

#endif /* SRC_HOMEWORK_5_GRAPHICAL_DOCUMENT_HPP */
