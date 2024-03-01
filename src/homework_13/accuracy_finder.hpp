#include "classifier.hpp"
#include <memory>

class AccuracyFinder {
public:
  AccuracyFinder();

  float GetAccuracy(const std::string &csv_path);

private:
  std::unique_ptr<Classifier> classifier_{};
};
