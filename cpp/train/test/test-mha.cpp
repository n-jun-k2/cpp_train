#include "../include/mha.h"
#include <gtest/gtest.h>

TEST(TestMhaCase, getAvailableOption) {

  auto options = mha::getAvailableOption();

  EXPECT_EQ(torch::kFloat32, options.dtype());
  EXPECT_EQ(torch::kStrided, options.layout());
  EXPECT_EQ(torch::kCPU, options.device().type());
  EXPECT_EQ(false, options.requires_grad());
}