#include "../include/genericpool.h"
#include  <gtest/gtest.h>
#include <vector>

struct TestObject {
  float X;
  float Y;
};

TEST(TestPool, VectorPool) {
  auto pPool = Pool<TestObject, std::vector>::createPool({TestObject(), TestObject()});
}

TEST(TestPool, VectorPoolGetInstance) {
  auto pPool = Pool<TestObject, std::vector>::createPool(5, TestObject());
}

TEST(TestPool, VectorPoolGetInstanceDeleter) {
  auto pPool = Pool<TestObject, std::vector>::createPool(5);
}