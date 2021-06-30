#include <iostream>
#include <type_traits>
#include <vector>
#include <memory>

struct widget {
  typedef int value_type;
  widget() = default;
  widget(int) {}
  void operator()(int){
    std::cout << "widget" << std::endl;
  }
};

template<class _T, class... Args, class = std::enable_if_t<std::is_invocable_v<_T, int>>>
void func(_T obj, Args&&... args) {
  std::cout << "関数" << std::endl;
}

template<class... Args>
void func(Args&&... args) {
  std::cout << "それ以外" << std::endl;
}



template<class _Ty, class = std::enable_if_t<std::is_pointer_v<_Ty> == true>>
std::remove_pointer_t<_Ty>* _GetReference(_Ty value) noexcept {
  return value;
}

template<class _Ty, class = std::enable_if_t<std::is_pointer_v<_Ty> == false>>
_Ty* _GetReference(_Ty& value) noexcept {
  return &value;
}



template<typename T, typename ...Args>
using is_class_constructible = std::conjunction<std::is_class<T>, std::is_constructible<T, Args...>>;

template<class _Ty, class = std::enable_if_t<std::is_pointer_v<_Ty>>>
void print(_Ty p) {
  std::cout << "pointer " << std::endl;
}

template<class _Ty, class... Args, class = std::enable_if_t<is_class_constructible<_Ty, Args...>::value>>
void print(_Ty& p) {
  std::cout << "object has constructor" << std::endl;
}

template<class _Ty,  class... Args, class = std::enable_if_t<is_class_constructible<_Ty, Args...>::value>>
void print(_Ty&& p) {
  std::cout << "object has constructor" << std::endl;
}

template<class _Ty>
struct Type{

  _Ty value;

  std::remove_pointer_t<_Ty>* get() {
    return _GetReference(this->value);
  }

};

struct Point {
  float x;
  float y;
};

template<class _T = Point*>
union WrapPoint{
  _T value;
  WrapPoint<_T>* next;
};


int main() {

  auto pPointerArray = std::vector<WrapPoint<>>(5);
  std::cout << pPointerArray[0].value << std::endl;
  std::cout << pPointerArray[0].value << std::endl;
  {
    std::shared_ptr<Point> pPointer(pPointerArray[0].value, [](Point* p) {
      std::cout << "delete Point pointer..." << std::endl;
      delete p;
    });
  }

  return 0;
}