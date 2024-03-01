#ifndef SRC_HOMEWORK_13_CLASSIFIER_HPP
#define SRC_HOMEWORK_13_CLASSIFIER_HPP

#include <eigen3/Eigen/Dense>

class Classifier {
public:
  using features_t = std::vector<float>;
  using probas_t = std::vector<float>;

  virtual ~Classifier() = default;

  [[nodiscard]] virtual std::size_t NumClasses() const = 0;

  [[nodiscard]] virtual std::size_t Predict(const features_t &) const = 0;

  [[nodiscard]] virtual probas_t PredictProba(const features_t &) const = 0;
};

class MlpClassifier : public Classifier {
public:
  MlpClassifier(const Eigen::MatrixXf &, const Eigen::MatrixXf &);

  [[nodiscard]] std::size_t NumClasses() const override;

  [[nodiscard]] std::size_t Predict(const features_t &) const override;

  [[nodiscard]] std::vector<float>
  PredictProba(const std::vector<float> &) const override;

private:
  Eigen::MatrixXf w1_, w2_;
};

#endif /* SRC_HOMEWORK_13_CLASSIFIER_HPP */
