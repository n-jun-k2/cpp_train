#include "../include/genericpool.h"

#include <iostream>
#include <typeinfo>

struct Point {
  double x;
  double y;
};

std::shared_ptr<std::shared_ptr<Point>> build(Pool<Point>& pointPool) {
  auto instance = pointPool.getInstance();

  std::cout << "create instance pointer..." << std::endl;
  instance->x = -1.0;
  instance->y = 1.0;

  struct _Deleter {
    explicit _Deleter() {}
    void operator()(std::shared_ptr<Point>* ptr) {
      std::cout << "delete pointer..." << std::endl;
      delete ptr;
    }
  };

  return std::shared_ptr<std::shared_ptr<Point>> (new std::shared_ptr<Point>(instance), _Deleter());
}

int main() {

  std::cout << "Point size: " << sizeof(Point) << std::endl;
  std::cout << "Resource<Point> size: " << sizeof(Resource<Point>) << std::endl;

  Resource<Point> item;
  Point point = static_cast<Point>(item.live);

  Pool<Point, std::vector> pointpool(5);
  for (auto buffer : pointpool.buffer.wrapper) {
    std::cout << "buffer :" << buffer.next << " : " << buffer.live.x << "," << buffer.live.y << std::endl;
  }

  std::cout << "------" << std::endl;
  for (int i = 0; i < pointpool.buffer.wrapper.size(); ++i) {
    std::cout << "adder :" << &pointpool.buffer.wrapper[i] << std::endl;
  }

  std::cout << "------" << std::endl;
  for (auto pointer = pointpool.firstAvailable; pointer != nullptr; pointer = pointer->next) {
    std::cout << "adder :" << pointer << std::endl;
  }

  std::cout << "------" << std::endl;
  {
    auto pPoint = build(pointpool);

    for (auto pointer = pointpool.firstAvailable; pointer != nullptr; pointer = pointer->next) {
      std::cout << "adder :" << pointer << std::endl;
    }
  }

  std::cout << "------" << std::endl;
  for (auto pointer = pointpool.firstAvailable; pointer != nullptr; pointer = pointer->next) {
    std::cout << "adder :" << pointer << std::endl;
  }

  {
    auto point1 = pointpool.getInstance();
    auto point2 = pointpool.getInstance();
    auto point3 = pointpool.getInstance();
    auto point4 = pointpool.getInstance();
    auto point5 = pointpool.getInstance();

    point1->x = 0.0;
    point1->y = 1.0;

    std::cout << "------" << std::endl;
    if (pointpool.firstAvailable == nullptr) std::cout << "empty" << std::endl;

    for (auto pointer = pointpool.firstAvailable; pointer != nullptr; pointer = pointer->next) {
      std::cout << "adder :" << pointer << std::endl;
    }
  }

  std::cout << "------" << std::endl;
  for (auto pointer = pointpool.firstAvailable; pointer != nullptr; pointer = pointer->next) {
    std::cout << "adder :" << pointer << std::endl;
  }

  return 0;
}