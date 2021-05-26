#include "../include/genericpool.h"

#include <iostream>
#include <typeinfo>

struct Point {
  double x;
  double y;
  Point(double _x, double _y):x(_x),y(_y){}
};

int main() {

  std::cout << "Point size: " << sizeof(Point) << std::endl; //Point size: 16
  std::cout << "Resource<Point> size: " << sizeof(Resource<Point>) << std::endl; // Resource<Point> size: 16

  auto pointpool = Pool<Point, std::vector>::createPool(5, Point(2,2));

  {
    auto point1 = pointpool->getInstance();
    std::cout << point1->x << ":" << point1->y << std::endl;
    auto point2 = pointpool->getInstance([](Point& ptr) {
      std::cout << "custom deleter... one" << std::endl;
    });
    std::cout << point2->x << ":" << point2->y << std::endl;
    auto point3 = pointpool->getInstance(2, 2);
    std::cout << point3->x << ":" << point3->y << std::endl;
    struct Deleter {
      void operator()(Point& ptr) {
        std::cout << "custom deleter... " << std::endl;
      }
    };
    auto point4 = pointpool->getInstance(Deleter(), 2, 2);
    std::cout << point4->x << ":" << point4->y << std::endl;
    auto point5 = pointpool->getInstance([](Point&){
      std::cout << "custom deleter lambda..." << std::endl;
    }, 2, 2);
    std::cout << point5->x << ":" << point5->y << std::endl;
  }

  using __type = uint64_t*;
  std::cout << "type size : " << sizeof(__type) << std::endl;
  auto intpool = Pool<__type>::createPool(5);

  {
    auto pInt1 = intpool->getInstance();
    *pInt1 = new uint64_t(10);
    std::cout << **pInt1 << std::endl;
    auto pInt2 = intpool->getInstance([](typename Pool<__type>::Element_Type &p){
      std::cout << "int custome deleter " << std::endl;
    });
    struct Deleter {
      void operator()(typename Pool<__type>::Element_Type& p) {
        std::cout << "int custom functor" << std::endl;
      }
    };
    auto pInt3 = intpool->getInstance(Deleter());
  }

  return 0;
}