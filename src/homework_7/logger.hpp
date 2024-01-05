#ifndef SRC_HOMEWORK_7_LOGGER_HPP
#define SRC_HOMEWORK_7_LOGGER_HPP

#include <fstream>
#include <string_view>

namespace homework_7 {

class Logger final {
 private:
  std::ofstream ofs_;

  Logger() = default;
  ~Logger() = default;

 public:
  Logger(const Logger &root) = delete;
  Logger &operator=(const Logger &) = delete;
  Logger(Logger &&root) = delete;
  Logger &operator=(Logger &&) = delete;

  static Logger &GetInstance() {
    static Logger instance;
    return instance;
  }

  /**
   * @brief Запись в файл лога
   * 
   * @param msg Данные, необходимые записать
   */
  void WriteToLogFile(std::string_view msg) {
    if (ofs_.is_open()) {
      ofs_ << msg;
    } else {
      static_assert(true, "Error write in file!");
    }
  }

  /**
   * @brief Создание файла для лога
   * 
   * @param filename Название файла
   */
  bool CreateFile(std::string_view filename) {
    ofs_.close();
    ofs_.open(filename.data(),
              std::fstream::in | std::fstream::out | std::fstream::trunc);

    return ofs_.is_open();
  }
};

}// namespace homework_7

#endif /* SRC_HOMEWORK_7_LOGGER_HPP */
