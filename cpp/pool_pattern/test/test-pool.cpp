#include "../include/genericpool.h"
#include  <gtest/gtest.h>
#include <vector>

struct TestObject {
  float X;
  float Y;
};

struct Test2Object{
  TestObject model;
  Test2Object() = default;
  Test2Object(float x, float y) {
    model.X = x;
    model.Y = y;
  }
};


TEST(TestPool, VectorPool) {
  auto pPool1 = Pool<TestObject, std::vector>::createPool(5);

  auto pPool2 = Pool<Test2Object, std::vector>::createPool(5);

}

TEST(TestPool, VectorPoolGetInstance) {
}

TEST(TestPool, VectorPoolGetInstanceDeleter) {

}