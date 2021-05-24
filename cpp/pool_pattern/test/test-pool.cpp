#include "../include/genericpool.h"
#include  <gtest/gtest.h>
#include <vector>

struct TestObject {
  float X;
  float Y;
};

struct Test2Object{
  TestObject model;
  Test2Object() = delete;
  Test2Object(float x, float y) {
    model.X = x;
    model.Y = y;
  }
};

struct Test3Object{
  TestObject model;
  Test3Object() {
    model.X = 1;
    model.Y = 1;
  }
  Test3Object(float x, float y) {
    model.X = x;
    model.Y = y;
  }
};

TEST(TestPool, VectorPool) {
  auto pPool1_1 = Pool<TestObject, std::vector>::createPool({TestObject(), TestObject()});
  auto pPool1_2 = Pool<TestObject, std::vector>::createPool(5, TestObject());
  auto pPool1_3 = Pool<TestObject, std::vector>::createPool(5);

  auto pPool2_1 = Pool<Test2Object, std::vector>::createPool(5, Test2Object(2, 2));
  auto pPool2_2 = Pool<Test2Object, std::vector>::createPool({Test2Object(2, 2), Test2Object(3, 3), Test2Object(4, 4)});

  auto pPool3_1 = Pool<Test3Object, std::vector>::createPool(5);
  auto pPool3_2 = Pool<Test3Object, std::vector>::createPool(5, Test3Object());
  auto pPool3_3 = Pool<Test3Object, std::vector>::createPool({Test3Object(), Test3Object(3, 3), Test3Object(), Test3Object()});
}

TEST(TestPool, VectorPoolGetInstance) {
}

TEST(TestPool, VectorPoolGetInstanceDeleter) {

}