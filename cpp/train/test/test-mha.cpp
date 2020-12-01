#include "../include/mha.h"
#include <array>
#include <gtest/gtest.h>

TEST(TestMhaCase, getAvailableOption) {

  auto options = mha::getAvailableOption();

  EXPECT_EQ(torch::kFloat32, options.dtype());
  EXPECT_EQ(torch::kStrided, options.layout());
  EXPECT_EQ(torch::kCPU, options.device().type());
  EXPECT_EQ(false, options.requires_grad());
}


TEST(TestMhaCase, rand) {

  auto min = 2;
  auto max = 3;

  auto expend = mha::rand(
    torch::ones({10}) * max,
    torch::ones({10}) * min,
    {10, 10},
    {}
  );

  ASSERT_TRUE((expend >= min).all().data<bool>());
  ASSERT_TRUE((expend <= max).all().data<bool>());
}

TEST(TestMhaCase, nonzero) {

  auto tensor = torch::tensor({
    0, 1, 3, 0,
    0, 0, 0, 0,
    0, 0, 0, 5,
    -1, 1, 2, 3,
  }).reshape({4, 4});

  auto expected = std::array<int, 4>{2, 0, 1, 4};

  auto test_list = mha::nonzero(tensor);

  const auto range = test_list.size();
  for(int i = 0; i < range; ++i)
    EXPECT_EQ(test_list[i].size(0), expected[i]);

}