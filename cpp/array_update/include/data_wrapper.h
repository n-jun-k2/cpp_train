#include "../include/data.h"
#include <vector>
#include <algorithm>
#include <functional>
#include <typeinfo>



namespace wrap {

  template <  typename TYPE, typename SIZE = int, template< typename E, typename Allocator = std::allocator<E> > class Container = std::vector>
  void containerToCPtr(SIZE& ref_size, TYPE** ptr_value, Container<TYPE>& container) {
    ref_size = static_cast<SIZE>(container.size());
    *ptr_value = nullptr;
    if (!container.empty()){
      *ptr_value = container.data();
    }
  }

  template < typename T, template< typename E, typename Allocator = std::allocator<E> > class Container = std::vector>
  void foreach(Container<T>& container, std::function<void(T&)> update, const int offset = 0, const unsigned int count = 0) {
    const auto n = count <= 0 ? container.size() : count;
    auto begin = std::begin(container);
    std::advance(begin, offset);
    std::for_each_n(begin, n, update);
  }

  template<class T>
  class IWrap{
    protected:
      T& value;
    public:
      IWrap(T& v) : value(v) {}
  };

  class ObjectWrap : IWrap<Object> {
    public:
      using IWrap<Object>::IWrap;
      ObjectWrap& name(const std::string& n) {
        this->value.name = n;
        return *this;
      }
      ObjectWrap& age(const int age) {
        this->value.age = age;
        return *this;
      }
      ObjectWrap& sex(const bool sex) {
        this->value.sex = sex;
        return *this;
      }
  };

  template<class T>
  class MethodChain {
    protected:
      std::function<void(T&)> action = __Initilize;
      static void __Initilize(T&){}

      void addAction(std::function<void(T&)> next) {
        const auto& prev = action;
        action = [=](auto& item){
          prev(item);
          next(item);
        };
      }

      void execute(T& self) {
        action(self);
      }

      void clear() {
        action = __Initilize;
      }

  };

  template < template< typename E, typename Allocator = std::allocator<E> > class Container = std::vector>
  class ManagerWrapper : MethodChain<ManagerWrapper<Container>>{
    public:
      Container<Room> vec_room;
      Container<Container<Object>> mtx_object;
      Container<Service> vec_service;
      Manager manager;

    public:
      explicit ManagerWrapper() = default;
      ManagerWrapper(const ManagerWrapper& other) = delete;
      ManagerWrapper& operator=(const ManagerWrapper& other) = delete;

      ManagerWrapper& roomInit(const int size) {
        this->addAction([=](auto& other){
          other.vec_room.resize(size);
          other.mtx_object.resize(size);
        });
        return *this;
      }
      ManagerWrapper& objectsInit(const int size, const int offset = 0, const unsigned int count = 0) {
        this->addAction([=](auto& other){
          foreach<Container<Object>>(other.mtx_object, [&](Container<Object>& item){
            item.resize(size);
          }, offset, count);
        });
        return *this;
      }
      ManagerWrapper& objectsUpdate(std::function<void(ObjectWrap&&)> update, const int offset = 0, const unsigned int count = 0) {
        this->addAction([=](auto& other){
          for (auto& row : other.mtx_object) {
            foreach<Object>(row, [&](Object& item) {
              update(ObjectWrap(item));
            }, offset, count);
          }
        });
        return *this;
      }
      ManagerWrapper& flush() {
        this->execute(*this);
        containerToCPtr(manager.roomCount, &manager.pRoomList, vec_room);
        containerToCPtr(manager.serviceCount, &manager.pServiceList, vec_service);
        for (auto i = 0; i < vec_room.size(); ++i) {
          containerToCPtr(vec_room[i].objectCount, &vec_room[i].pObjectList, mtx_object[i]);
        }
        this->clear();
        return *this;
      }
  };

}