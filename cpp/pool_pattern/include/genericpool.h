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
  ~WrapContainer() {}
};

template<
  typename E,
  template<typename T, typename Allocator = std::allocator<T>>class Container = std::vector>
struct Pool {

  WrapContainer<E, Container> buffer;
  Resource<E>* firstAvailable;

  Pool() = delete;
  ~Pool() = default;

  explicit Pool(const uint32_t size) :buffer(){
    buffer.wrapper.resize(size);
    firstAvailable = &buffer.wrapper[0];

    auto it = std::begin(buffer.wrapper);
    auto end = std::prev(std::end(buffer.wrapper), 2);
    for(; it <= end; it = std::next(it)){
      it->next = &(*std::next(it));
    }
    std::prev(std::end(buffer.wrapper))->next = nullptr;
  }

  std::shared_ptr<E> getInstance() {
    assert(firstAvailable != nullptr);

    std::add_pointer_t<E> element = &firstAvailable->live;
    firstAvailable = firstAvailable->next;

    struct _Release {
      Pool& pool;
      explicit _Release(Pool& pPool)
        : pool(pPool) {}
      void operator()(E* pItem){
        std::cout << "release..." << std::endl;
        auto releaseResource = reinterpret_cast<Resource<E>*>(pItem);

        if(pool.firstAvailable == nullptr) {
          pool.firstAvailable = releaseResource;
          pool.firstAvailable->next = nullptr;
          return;
        }

        if (releaseResource < pool.firstAvailable) {
          releaseResource->next = pool.firstAvailable;
          pool.firstAvailable = releaseResource;
        } else {
          releaseResource->next = pool.firstAvailable->next;
          pool.firstAvailable->next = releaseResource;
        }
      }
    };

    return std::shared_ptr<E>(element, _Release(*this));
  }

  E* data() {
    return buffer.raw.data();
  }

};