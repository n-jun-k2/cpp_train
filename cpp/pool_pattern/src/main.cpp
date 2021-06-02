#include "../include/genericpool.h"

#include <iostream>
#include <typeinfo>

struct Point {
  double x;
  double y;
  Point() :x(0), y(0) {
    std::cout << "----point instance!!-----" << std::endl;
  }
  Point(double _x, double _y):x(_x),y(_y){
    std::cout << "----point instance!!-----" << std::endl;
  }
  ~Point() {
    std::cout << "----point delete!!------" << std::endl;
  }
  Point(const Point& p) {
    std::cout << "copy constructor"  << std::endl;
  }
  Point& operator=(const Point& p) {
    std::cout << "operator copy" << std::endl;
    return *this;
  }
};

template<typename _T, typename _D, typename _I>
std::shared_ptr<std::remove_pointer_t<_T>> create_ptr(WrapContainer<_T, std::vector>& container, _I init, _D deleter) {
  auto pointer = _GetReference(container.wrapper[0].live);
  init(pointer);
  struct _R {
    _D d;
    explicit _R(_D del) : d(std::move(del)) {}
    void operator()(std::remove_pointer_t<_T>* p) {
      d(p);
      std::cout << "wrap delete." << std::endl;
    }
  };
  return std::shared_ptr<std::remove_pointer_t<_T>>(pointer, _R(deleter));
}

template<typename _T, typename _D, typename _I>
std::shared_ptr<std::remove_pointer_t<_T>> wrap_create_ptr(WrapContainer<_T, std::vector>& container, _I init, _D deleter) {
  return create_ptr(container, std::move(init), std::move(deleter));
}

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

  using __type = Point*;
  auto intpool = Pool<__type>::createPool(5);

  {
    auto pInt1 = intpool->getInstance([](typename Pool<__type>::Element_Type &p) {
      std::cout << "init custome deleter..." << std::endl;
      delete p;
    }, 11, 12);
    auto a = pInt1.get();
    std::cout << " a:" << a->x << ":" << a->y << std::endl;

    auto pPoint = new Point(20, 30);
    auto pInt2 = intpool->getInstance([](typename Pool<__type>::Element_Type &p) {
      std::cout << "init custome deleter..." << std::endl;
      delete p;
    }, pPoint);
    auto a2 = pInt2.get();
    std::cout << " a2:" << a2->x << ":" << a2->y << std::endl;
  }

  return 0;
}