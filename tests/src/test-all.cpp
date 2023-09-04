#include <gtest/gtest.h>

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  // ::testing::GTEST_FLAG(filter) = "Homework2.*";
  // ::testing::GTEST_FLAG(filter) = "Homework3.*";
  // ::testing::GTEST_FLAG(filter) = "Homework4.*";
  ::testing::GTEST_FLAG(filter) = "Homework5.*";

  return RUN_ALL_TESTS();
}