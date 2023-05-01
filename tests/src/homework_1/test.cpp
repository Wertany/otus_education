#include <gtest/gtest.h>

#include "homework_1/homework_1.hpp"

namespace {

TEST(Homework_1, HomeworkTestGoogle) {
  ASSERT_TRUE(version() > 0);
}

}  // namespace