#pragma once
#include <iostream>
#include <cassert>
#include <vector>
#include <memory>
#include <type_traits>

template<typename E>
union Resource {
  std::remove_pointer_t<E> live;
  std::add_pointer_t<Resource> next;
};

template<typename E,
  template<typename T, typename Allocator = std::allocator<T>>class Container = std::vector>
union WrapContainer{
  Container<Resource<E>> wrapper;
  Container<E> raw;

  WrapContainer() : wrapper() {}
  WrapContainer(const uint32_t size) :
    raw(size) {}
  WrapContainer(std::initializer_list<Resource<E>> init) :
    wrapper(std::begin(init), std::end(init)) {}
  WrapContainer(std::initializer_list<E> init) :
    raw(std::begin(init), std::end(init)) {}
  WrapContainer(const uint32_t n, const Resource<E>& value) :
    wrapper(n, value){}
  WrapContainer(const uint32_t n, const E& value) :
    raw(n, value){}
  ~WrapContainer() {}
};

template<typename E, template<typename T, typename Allocator = std::allocator<T>>class Container = std::vector>
class Pool : public std::enable_shared_from_this<Pool<E, Container>> {

  using Element_Type = std::remove_pointer_t<E>;

  WrapContainer<Element_Type, Container> buffer;
  Resource<Element_Type>* firstAvailable;


  void _init() noexcept {
    firstAvailable = &buffer.wrapper[0];

    auto it = std::begin(buffer.wrapper);
    auto end = std::prev(std::end(buffer.wrapper), 2);
    for(; it <= end; it = std::next(it)){
      it->next = &(*std::next(it));
    }
    std::prev(std::end(buffer.wrapper))->next = nullptr;
  }

  void _Releasepool(Resource<Element_Type>* item) noexcept {

    if(firstAvailable == nullptr) {
      firstAvailable = item;
      firstAvailable->next = nullptr;
      return;
    }

    if (item < firstAvailable) {
      item->next = firstAvailable;
      firstAvailable = item;
    } else {
      item->next = firstAvailable->next;
      firstAvailable->next = item;
    }
  }

  template <typename _Rx, typename... _Args>
  std::shared_ptr<Element_Type> _getInstance(_Rx rel, _Args&& ...args) {
    assert(firstAvailable != nullptr);

    std::add_pointer_t<Element_Type> element = &firstAvailable->live;
    firstAvailable = firstAvailable->next;

    *element = Element_Type(std::forward<_Args>(args)...);

    return std::shared_ptr<Element_Type>(element, std::move(rel));
  }

public:

  Pool() = delete;
  ~Pool() = default;

  explicit Pool(const uint32_t size) noexcept
    :buffer(size){this->_init();}
  explicit Pool(std::initializer_list<Resource<E>> il) noexcept
    :buffer(il){this->_init();}
  explicit Pool(std::initializer_list<E> il) noexcept
    :buffer(il){this->_init();}
  explicit Pool(const uint32_t n, const Resource<E>& value) noexcept
    :buffer(n, value) {this->_init();}
  explicit Pool(const uint32_t n, const E& value) noexcept
    :buffer(n, value) {this->_init();}

  static std::shared_ptr<Pool<Element_Type, Container>> createPool(const uint32_t size) noexcept{
    return std::make_shared<Pool<Element_Type, Container>>(size);
  }
  static std::shared_ptr<Pool<Element_Type, Container>> createPool(std::initializer_list<Resource<E>> il) noexcept {
    return std::make_shared<Pool<Element_Type, Container>>(il);
  }
  static std::shared_ptr<Pool<Element_Type, Container>> createPool(std::initializer_list<E> il) noexcept {
    return std::make_shared<Pool<Element_Type, Container>>(il);
  }
  static std::shared_ptr<Pool<Element_Type, Container>> createPool(const uint32_t n, const Resource<E>& value) noexcept {
    return std::make_shared<Pool<Element_Type, Container>>(n, value);
  }
  static std::shared_ptr<Pool<Element_Type, Container>> createPool(const uint32_t n, const E& value) noexcept {
    return std::make_shared<Pool<Element_Type, Container>>(n, value);
  }

  template<typename _Dx, typename... _Args, class = std::enable_if_t<std::is_invocable_v<_Dx, Element_Type&>>>
  std::shared_ptr<Element_Type> getInstance(_Dx deleter, _Args&& ...args) {
    struct _CRelease {
      std::shared_ptr<Pool<Element_Type, Container>> pool;
      _Dx del;
      explicit _CRelease(std::shared_ptr<Pool<Element_Type, Container>> pPool, _Dx _del)
        : pool(pPool), del(std::move(_del)) {}
      void operator()(Element_Type* pItem){
        auto releaseResource = reinterpret_cast<Resource<Element_Type>*>(pItem);
        del(releaseResource->live);
        pool->_Releasepool(releaseResource);
      }
    };
    return this->_getInstance(_CRelease(this->shared_from_this(), std::move(deleter)), args...);
  }

  template<typename... _Args>
  std::shared_ptr<Element_Type> getInstance(_Args&& ...args) {
    struct _Release {
      std::shared_ptr<Pool<Element_Type, Container>> pool;
      explicit _Release(std::shared_ptr<Pool<Element_Type, Container>> pPool)
        : pool(pPool) {}
      void operator()(Element_Type* pItem){
        auto releaseResource = reinterpret_cast<Resource<Element_Type>*>(pItem);
        pool->_Releasepool(releaseResource);
      }
    };
    return this->_getInstance(_Release(this->shared_from_this()), args...);
  }

  E* data() {
    return buffer.raw.data();
  }

};