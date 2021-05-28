#include <iostream>
#include <type_traits>


struct widget {
  typedef int value_type;
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


template<class _Ty>
struct Type{

  _Ty value;

  std::remove_pointer_t<_Ty>* get() {
    return _GetReference(this->value);
  }

};

int main() {
  std::cout << std::is_invocable_v<widget> << std::endl;

  func(widget(), 0, 0);
  func(0, 0, 0);

  Type<int> a;
  a.value = 10;
  auto pA = a.get();
  std::cout << pA << ":" << *pA << std::endl;

  Type<int*> b;
  b.value = new int(5);
  auto pB = b.get();
  std::cout << pB << ":" << *pB << std::endl;
  delete b.value;

  return 0;
}