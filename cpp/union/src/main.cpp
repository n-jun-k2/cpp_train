#include <iostream>
#include <vector>
#include <memory>
#include <type_traits>

struct Point2D {
  float x;
  float y;
};
static_assert(std::is_pod<Point2D>::value, "Should be POD.");

class Point3D {
  private:
    Point2D pos;
    float Z;
  public:
    Point3D& x(float x) {
      pos.x = x;
      return *this;
    }

    Point3D& y(float y) {
      pos.y = y;
      return *this;
    }

    Point3D& z(float z) {
      Z = z;
      return *this;
    }

    operator Point2D() const {
      return pos;
    }

    operator Point2D*() {
      return &pos;
    }
};

template< template<typename E, typename Allocate = std::allocator<E>> class Container = std::vector>
using Point3DList = Container<Point3D>;

static_assert(std::is_pod<Point3D>::value, "Should be POD.");
static_assert(std::is_convertible<Point3D, Point2D>::value, "Success.");

void func (Point3DList<>& list) {
  Point2D *pData = *list.data();
  for (auto i = 0; i < list.size(); ++i)
    std::cout << pData[i].x << ":" << pData[i].y << std::endl;
}


int main() {
  std::cout << "Hello World" << std::endl;

  auto position_list = std::vector<Point3D> {
    Point3D().x(1).y(4),
    Point3D().x(10).y(5),
  };

  func(position_list);

  std::cout << "--------" << std::endl;

  auto pPosition_list = std::make_shared<std::vector<Point3D>>(std::vector<Point3D>{
    Point3D().x(2).y(4).z(11),
    Point3D().x(6).y(8).z(32),
    Point3D().x(10).y(12),
  });

  func(*pPosition_list);

  return 0;
}