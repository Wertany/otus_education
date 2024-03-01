#include "classifier.hpp"

#include <cmath>
#include <utility>

using Eigen::MatrixXf;
using Eigen::VectorXf;

namespace {

template <typename T> auto sigma(T x) { return 1 / (1 + std::exp(-x)); }

VectorXf sigmav(const VectorXf &v) {
  VectorXf res{v.rows()};
  for (auto i = 0; i < v.rows(); ++i) {
    res(i) = sigma(v(i));
  }
  return res;
}

VectorXf softmax(const VectorXf &v) {
  VectorXf res{v.rows()};
  float denominator = 0.0F;

  for (auto i = 0; i < v.rows(); ++i) {
    denominator += std::exp(v(i));
  }
  for (auto i = 0; i < v.rows(); ++i) {
    res(i) = std::exp(v(i)) / denominator;
  }
  return res;
}

} // namespace
MlpClassifier::MlpClassifier(const MatrixXf &w1, const MatrixXf &w2)
    : w1_{std::move(w1)}, w2_{std::move(w2)} {}

std::size_t MlpClassifier::NumClasses() const { return w2_.cols(); }

std::size_t MlpClassifier::Predict(const features_t &feat) const {
  auto proba = PredictProba(feat);
  auto argmax = std::max_element(proba.begin(), proba.end());
  return std::distance(proba.begin(), argmax);
}

MlpClassifier::probas_t
MlpClassifier::PredictProba(const features_t &feat) const {
  VectorXf x{feat.size()};
  for (std::size_t i = 0; i < feat.size(); ++i) {
    x[i] = feat[i] / 255;
  }

  auto o1 = sigmav(w1_ * x);
  auto o2 = softmax(w2_ * o1);

  probas_t res;
  for (auto i = 0; i < o2.rows(); ++i) {
    res.push_back(o2(i));
  }
  return res;
}