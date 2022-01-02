#include "row.h"

#include <iostream>
#include <vector>
#include <string>
#include <functional>



struct CreateCObject {
  CLASS_C m_instanceC;
  std::vector<CLASS_A> m_instanceA;
  std::vector<CLASS_B> m_instanceB;

  struct update{};
  struct type {int value;};
  struct priority {float value;};

  CreateCObject& operator<<(type&& type){
    m_instanceC.type = type.value;
    return *this;
  }
  CreateCObject& operator<<(priority&& priority) {
    m_instanceC.priority = priority.value;
    return *this;
  }
  CreateCObject& operator<<(std::function<void(std::vector<CLASS_A>&)> update) {
    update(m_instanceA);
    return *this;
  }
  CreateCObject& operator<<(std::function<void(std::vector<CLASS_B>&)> update) {
    update(m_instanceB);
    return *this;
  }
  void operator<<(const update _) {
    m_instanceC.a_count = m_instanceA.size();
    m_instanceC.p_a = m_instanceA.data();
    m_instanceC.b_count = m_instanceB.size();
    m_instanceC.p_b = m_instanceB.data();
  }
};

CreateCObject::type operator""Type (unsigned long long v) {
  return CreateCObject::type{static_cast<int>(v)};
}

CreateCObject::priority operator""Priority (long double v) {
  return CreateCObject::priority{static_cast<float>(v)};
}

std::string get_string() {
  return std::string("function return.");
}

template<class T>
class TextDisplayer {
  public:
    explicit TextDisplayer(T&& text)
    :text_(std::forward<T>(text)){}
  private:
    const T text_;
};

// deduction guide
template<class T>TextDisplayer(T&&) -> TextDisplayer<T>;

int main() {

  CreateCObject instance = {};
  instance
    << 10Type
    << 0.5Priority
    << [](std::vector<CLASS_B>& vec){
      const int size = 10;
      vec.resize(size);
      for(auto& v : vec) {
        v.b = 0.5f;
        v.r = 0.8f;
        v.g = 1.0f;
      }
    }
    << [](std::vector<CLASS_A>& vec){
      const int size = 5;
      vec.resize(size);
      for(auto& v : vec) {
        v.x = 1;
        v.y = 2;
        v.z = 3;
      }
    }
    << CreateCObject::update();

  std::cout << instance.m_instanceC.type << std::endl;
  std::cout << instance.m_instanceC.priority << std::endl;
  std::cout << instance.m_instanceC.a_count << std::endl;
  for (auto i = 0; i < instance.m_instanceC.a_count; ++i) {
    auto a = instance.m_instanceC.p_a[i];
    std::cout << a.x
              << ","
              << a.y
              << ","
              << a.z
              << std::endl;
  }
  std::cout << instance.m_instanceC.b_count << std::endl;
  for (auto i = 0; i < instance.m_instanceC.b_count; ++i) {
    auto b = instance.m_instanceC.p_b[i];
    std::cout << b.b << ","
              << b.g << ","
              << b.r
              << std::endl;
  }

  std::string txt = "Hello World";
  TextDisplayer display1(txt);
  TextDisplayer display2(get_string());
  return 0;
}