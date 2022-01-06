#include "row.h"

#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <variant>

using instance_type = std::variant<std::vector<CLASS_A>, std::vector<CLASS_B>>;
template<class> struct range;
template<class R, class... ArgTypes>
struct range <R(ArgTypes...)>
{const unsigned int size; std::function<R(ArgTypes...)> func;};
template<class T>
using vector_range = range<void(std::vector<T>&)>;

struct CREATER;

template<class T>
class IWRAP {
protected:
  T& obj;
public:
  explicit IWRAP(T& b): obj(b){}
};

class CLASS_A_WRAP : public IWRAP<CLASS_A>{
public:
  using IWRAP<CLASS_A>::IWRAP;
  void update() {
    this->obj.x = 10;
    this->obj.y = 5;
    this->obj.z = 1;
  }
};
class CLASS_B_WRAP : public IWRAP<CLASS_B>{
public:
  using IWRAP<CLASS_B>::IWRAP;
  void update(){
    this->obj.r = 0.5f;
    this->obj.g = 0.1f;
    this->obj.b = 2.5f;
  }
};

class CLASS_C_WRAP {
  private:
    CREATER& m_obj;
    CLASS_C& m_c;
    instance_type& m_instance_type;
  private:
    template<class TWrap, class T>
    void __range_func(vector_range<TWrap> create) {
      auto row_array = std::vector<T>(create.size);
      auto wrap_array = std::vector<TWrap>();
      for(auto i = 0; i < create.size; ++i)
        wrap_array.emplace_back(row_array[i]);
      create.func(wrap_array);
      m_instance_type = row_array;
    }
    struct visitor {
      CLASS_C& m_c;
      void operator()(std::vector<CLASS_A>& a) {
        m_c.a_count = a.size();
        m_c.p_a = a.data();
      }
      void operator()(std::vector<CLASS_B>& b) {
        m_c.b_count = b.size();
        m_c.p_b = b.data();
      }
    };

  public:
    struct update{};
    struct type {int value;};
    struct priority {float value;};

    explicit CLASS_C_WRAP(CREATER& a, CLASS_C& b, instance_type& c)
    :m_obj(a), m_c(b), m_instance_type(c){}

    CLASS_C_WRAP& operator<<(type&& type){
      m_c.type = type.value;
      return *this;
    }
    CLASS_C_WRAP& operator<<(priority&& priority) {
      m_c.priority = priority.value;
      return *this;
    }
    CLASS_C_WRAP& operator<<(const vector_range<CLASS_A_WRAP>& range){
      __range_func<CLASS_A_WRAP, CLASS_A>(range);
      return *this;
    }
    CLASS_C_WRAP& operator<<(const vector_range<CLASS_B_WRAP>& range){
      __range_func<CLASS_B_WRAP, CLASS_B>(range);
      return *this;
    }
    CREATER& operator<<(const update _) {
      std::visit(visitor{m_c}, m_instance_type);
      return m_obj;
    }
};

struct CREATER {
  std::vector<CLASS_C> m_instanceC;
  std::vector<instance_type> m_instance;

  CREATER& operator<<(const vector_range<CLASS_C_WRAP>& _range) {
    m_instanceC.resize(_range.size);
    m_instance.resize(_range.size);

    std::vector<CLASS_C_WRAP> items;
    for(auto i = 0; i < _range.size; ++i)
      items.emplace_back(*this, m_instanceC[i], m_instance[i]);

    _range.func(items);
    return *this;
  }

};

CLASS_C_WRAP::type operator""_Type (unsigned long long v) {
  return CLASS_C_WRAP::type{static_cast<int>(v)};
}

CLASS_C_WRAP::priority operator""_Priority (long double v) {
  return CLASS_C_WRAP::priority{static_cast<float>(v)};
}



int main() {
  unsigned int c_array_size = 10;
  CREATER factory = {};
  factory << vector_range<CLASS_C_WRAP> {
    c_array_size,
    [=](std::vector<CLASS_C_WRAP>& c_array)
    {
      for(int i = 0; i < c_array_size; ++i) {
        auto c = c_array[i];
        if (i % 2 == 0)
          c << 10_Type << 0.4_Priority << vector_range<CLASS_A_WRAP> {5, [](std::vector<CLASS_A_WRAP>& a_array){
            for(auto& a : a_array)a.update();
          }}
          << CLASS_C_WRAP::update();
        else
          c << 6_Type << 0.1_Priority << vector_range<CLASS_B_WRAP> {3, [](std::vector<CLASS_B_WRAP>& b_array){
            for(auto& b : b_array) b.update();
          }}
          << CLASS_C_WRAP::update();
      }
    }
  };

  auto& c_array = factory.m_instanceC;
  int i = 0;
  for (auto& c : c_array) {
    std::cout << ++i << ": type :" << c.type << ", a_count :" << c.a_count << ", b_count :" << c.b_count << std::endl;
  }

  return 0;
}