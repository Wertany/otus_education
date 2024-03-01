#include "accuracy_finder.hpp"
#include <cstddef>
#include <filesystem>
#include <fstream>
#include <memory>

namespace {
Eigen::MatrixXf read_mat_from_file(std::size_t rows, std::size_t cols,
                                   const std::filesystem::path &mat_path) {
  std::ifstream mat_file(mat_path.native());
  Eigen::MatrixXf mat_res(rows, cols);

  for (std::size_t i = 0; i < rows; ++i) {
    for (std::size_t j = 0; j < cols; ++j) {
      std::string tmp;
      mat_file >> tmp;
      double val = std::stod(tmp);
      mat_res(i, j) = static_cast<float>(val);
    }
  }

  return mat_res;
}

void read_features(std::vector<std::string> &pixels,
                   Classifier::features_t &features) {
  features.clear();
  for (const auto &pixel : pixels) {
    features.push_back(std::stof(pixel));
  }
}

std::vector<std::string> Split(const std::string &str, char d) {
  std::vector<std::string> v;
  if (!str.empty()) {
    size_t pos = 0;
    size_t pos_last = 0;
    while ((pos = str.find(d, pos_last)) != std::string::npos) {
      if (pos != 0) {
        if ((pos - pos_last) > 0) {
          v.emplace_back(str.substr(pos_last, pos - pos_last));
        }
        pos_last = ++pos;
      } else {
        ++pos_last;
      }
    }

    if (pos == std::string::npos) {
      size_t sz = str.size();
      if (pos_last < sz) {
        v.emplace_back(str.substr(pos_last, sz - pos_last));
      }
    }
  }
  return v;
}

std::vector<std::vector<std::string>>
read_csv(const std::filesystem::path &csvPath) {
  std::vector<std::vector<std::string>> result;
  std::fstream csv(csvPath);

  for (std::string line; std::getline(csv, line);) {
    result.push_back(Split(line, ','));
  }

  return result;
}

constexpr std::size_t kInput{784};
constexpr std::size_t kHidden{128};
constexpr std::size_t kOutput{10};
const auto kPathW1 = std::filesystem::current_path() / "w1.txt";
const auto kPathW2 = std::filesystem::current_path() / "w2.txt";
} // namespace

AccuracyFinder::AccuracyFinder() {
  auto l1 = read_mat_from_file(kInput, kHidden, kPathW1);
  auto l2 = read_mat_from_file(kHidden, kOutput, kPathW2);

  classifier_ = std::make_unique<MlpClassifier>(l1.transpose(), l2.transpose());
}

float AccuracyFinder::GetAccuracy(const std::string &csv_path) {
  MlpClassifier::features_t features;

  auto file = read_csv(csv_path);
  auto all = static_cast<float>(file.size());
  float right = 0;

  for (auto &file_string : file) {
    std::size_t expected = std::stoi(file_string.front());
    file_string.erase(file_string.begin());

    read_features(file_string, features);
    auto pred = classifier_->Predict(features);

    if (expected == pred) {
      right++;
    }
  }

  return right / all;
}
