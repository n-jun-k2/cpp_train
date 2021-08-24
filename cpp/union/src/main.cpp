#include <iostream>
#include <vector>
#include <memory>
#include <type_traits>

struct Point2D {
  float x;
  float y;
};

struct Point2DF {
  float x;
  float y;
  explicit Point2DF() = default;
  Point2DF(float x, float y): x(x), y(y) {}
  ~Point2DF() = default;
};

template<class T>
union Node {
  T live;
  std::add_pointer_t<T> next;
};

int main() {
  std::cout << "Hello World" << std::endl;

  Node<Point2D> node;
  Node<Point2DF> node_f;

  return 0;
}