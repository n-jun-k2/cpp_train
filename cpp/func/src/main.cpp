#include "../include/sum.h"
#include <iostream>
#include <memory>

struct IResource {
  constexpr IResource() noexcept = default;
  virtual ~IResource() noexcept {}
  IResource(const IResource&) = delete;
  IResource& operator=(const IResource&) = delete;
  virtual void destroy() noexcept = 0;
  virtual void destroy_this() noexcept = 0;
};

template<class T>
struct SimpleResource : public IResource {
  T* value;
  SimpleResource(T* value):value(value){}
  virtual void destroy() noexcept override {
    delete value;
  }
  virtual void destroy_this() noexcept override {
    delete this;
  }
};


template<class T, class D>
struct CustomeResource : public IResource{
  std::pair<T*, D> com_pair;
  CustomeResource(T* value, D destroy): com_pair(value, std::move(destroy)){}
  virtual void destroy() noexcept override {
    com_pair.second(com_pair.first);
  }
  virtual void destroy_this() noexcept override {
    delete this;
  }
};

template<class _Ty>
struct Manager {
  using element_type = std::remove_pointer_t<_Ty>;

  element_type* value{nullptr};
  IResource* resource{nullptr};

  template <class T>
  explicit Manager(T* _Px) {
    resource = new SimpleResource(_Px);
  }

  template <class T, class _Dx>
  Manager(T* _Px, _Dx _Dt) {
    resource = new CustomeResource(_Px, std::move(_Dt));
  }

  ~Manager() noexcept {
    resource->destroy();
    resource->destroy_this();
  }
};

int main() {
  std::cout << "Hello World" << std::endl;

  Manager<int> obj(new int(2), [](int* ptr){
    std::cout << "FUNC" << std::endl;
    delete ptr;
  });

  std::cout << sizeof(int) << std::endl;
  std::cout << sizeof(obj) << std::endl;


  return 0;
}