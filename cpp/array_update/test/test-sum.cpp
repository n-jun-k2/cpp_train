#include "../include/sum.h"
#include  <gtest/gtest.h>

//テストコード
TEST(TestCase, sum) {
  EXPECT_EQ(2, sum(1,1));        //OK
  EXPECT_EQ(1000, sum(123,877)); //OK
  EXPECT_EQ(-8, sum(-12,3));     //NG
}
