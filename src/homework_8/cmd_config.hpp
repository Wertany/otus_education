#ifndef SRC_HOMEWORK_8_ARGUMENT_PARSER_HPP
#define SRC_HOMEWORK_8_ARGUMENT_PARSER_HPP

#include <boost/filesystem.hpp>
#include <boost/filesystem/directory.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/program_options.hpp>
#include <boost/regex.hpp>
#include <cstddef>
#include <iostream>
#include <string>
#include <vector>

namespace homework_8 {

enum class HashType { kMD5, kCRC32 };

struct CMDConfig {
  bool ParseArguments(int argc, char *argv[]);// NOLINT

  std::vector<std::string> dirs;
  std::vector<std::string> excluded_dirs;
  std::size_t scan_levels{0};
  std::size_t min_size_file{1};
  std::size_t size_block{50};
  std::vector<boost::regex> file_masks;
  HashType hash_type;
};

inline bool CMDConfig::ParseArguments(int argc, char *argv[]) {// NOLINT
  namespace po = boost::program_options;
  po::options_description desc("All options");
  desc.add_options()("dirs,d",
                     po::value<std::vector<std::string>>()->required()->multitoken(),
                     "директории для сканирования")(
      "excluded_dirs,e", po::value<std::vector<std::string>>()->multitoken(),
      "директории для исключения из сканирования")(
      "scan_levels,s", po::value<std::size_t>()->required()->default_value(0),
      "уровень сканирования")("min_size_file,m",
                              po::value<std::size_t>()->required()->default_value(1),
                              "минимальный размер файла")(
      "size_block,b", po::value<std::size_t>()->required()->default_value(50),
      "размер блока, которым производится чтения файлов")(
      "file_mask,f", po::value<std::vector<std::string>>()->multitoken(),
      "маски имен файлов, разрешенных для сравнения")(
      "hash,h", po::value<std::string>()->required()->default_value("md5"),
      "алгоритм хэширования (md5, crc32)")("help", "справка");

  po::variables_map var_map;
  try {
    auto parsed = po::command_line_parser(argc, argv).options(desc).run();
    po::store(parsed, var_map);
    if (var_map.count("help") != 0U) {
      std::cout << desc << '\n';
      return false;
    }
    po::notify(var_map);
  } catch (const po::error &e) {
    std::cerr
        << "Ошибка парсинга аргументов: " << e.what()
        << "\nПожалуйста, используйте команду --help для просмотра допустимых аргументов";
    return false;
  }

  namespace fs = boost::filesystem;

  if (var_map.count("dirs") != 0U) {
    auto input_dirs = std::move(var_map["dirs"].as<std::vector<std::string>>());
    for (auto &dir : input_dirs) {
      fs::path path(dir);
      if (fs::is_directory(path)) { dirs.push_back(fs::canonical(path).string()); }
    }
  }

  if (var_map.count("excluded_dirs") != 0U) {
    auto input_excluded_dirs =
        std::move(var_map["excluded_dirs"].as<std::vector<std::string>>());
    for (auto &excluded_dir : input_excluded_dirs) {
      fs::path path(excluded_dir);
      if (fs::is_directory(path)) {
        excluded_dirs.push_back(fs::canonical(path).string());
      }
    }
  }

  if (var_map.count("scan_levels") != 0U) {
    scan_levels = var_map["scan_levels"].as<std::size_t>();
  }

  if (var_map.count("min_size_file") != 0U) {
    min_size_file = var_map["min_size_file"].as<std::size_t>();
  }

  if (var_map.count("file_mask") != 0U) {
    auto file_masks_input =
        std::move(var_map["file_mask"].as<std::vector<std::string>>());
    for (const auto &mask : file_masks_input) {
      file_masks.emplace_back(mask, boost::regex_constants::ECMAScript);
    }
  }

  if (var_map.count("hash") != 0U) {
    auto string_hash = var_map["hash"].as<std::string>();

    if (string_hash == "md5") {
      hash_type = HashType::kMD5;
    } else if (string_hash == "crc32") {
      hash_type = HashType::kCRC32;
    }
  }

  return true;
}


}// namespace homework_8

#endif /* SRC_HOMEWORK_8_ARGUMENT_PARSER_HPP */
