#pragma once

#include <cassert>
#include <vector>
#include <memory>
#include <type_traits>

template <class _Ty, typename... _Args>
using is_class_constructible = std::conjunction<std::is_class<_Ty>, std::is_constructible<_Ty, _Args...>>;

template <typename E>
union Resource
{
  E live;
  std::add_pointer_t<Resource> next;
};

template <typename E, template <typename T, typename Allocator = std::allocator<T>> class Container = std::vector>
class Pool : public std::enable_shared_from_this<Pool<E, Container>>
{
public:
  using Element_Type = E;
  using Element_NonPointer_Type = std::remove_pointer_t<Element_Type>;
  using Buffer = Container<Resource<E>>;
  using InstancePtr = std::shared_ptr<Element_NonPointer_Type>;

private:
  Buffer buffer;
  Resource<Element_Type> *firstAvailable;

  void _init() noexcept
  {
    firstAvailable = &buffer[0];

    auto it = std::begin(buffer);
    auto end = std::prev(std::end(buffer), 2);
    for (; it <= end; it = std::next(it))
    {
      it->next = &(*std::next(it));
    }
    std::prev(std::end(buffer))->next = nullptr;
  }

  void _Releasepool(Resource<Element_Type> *item) noexcept
  {

    if (firstAvailable == nullptr)
    {
      firstAvailable = item;
      firstAvailable->next = nullptr;
      return;
    }

    if (item < firstAvailable)
    {
      item->next = firstAvailable;
      firstAvailable = item;
    }
    else
    {
      item->next = firstAvailable->next;
      firstAvailable->next = item;
    }
  }

  template <class _T = Element_Type, class _Rx,
            std::enable_if_t<std::is_pointer_v<_T>> * = nullptr>
  InstancePtr make_shared(_T *element, _Rx rel)
  {
    return InstancePtr(*element, std::move(rel));
  }

  template <class _T = Element_Type, class _Rx,
            std::enable_if_t<!std::is_pointer_v<_T>> * = nullptr>
  InstancePtr make_shared(_T *element, _Rx rel)
  {
    return InstancePtr(element, std::move(rel));
  }

  template <typename _Rx, typename _InitFunc>
  InstancePtr __getInstance(_Rx rel, _InitFunc init)
  {
    assert(firstAvailable != nullptr);

    auto element = &firstAvailable->live;
    firstAvailable = firstAvailable->next;
    init(element);
    return make_shared(element, std::move(rel));
  }

  template <typename _T = Element_Type, typename _Rx, typename... _Args,
            std::enable_if_t<is_class_constructible<_T, _Args...>::value> * = nullptr>
  InstancePtr _getInstance(_Rx rel, _Args &&...args)
  {
    return this->__getInstance(std::move(rel), [&](_T *ptr)
                               { *ptr = _T(std::forward<_Args>(args)...); });
  }

  template <typename _T = Element_Type, typename _Rx, typename... _Args,
            std::enable_if_t<std::is_pointer_v<_T>> * = nullptr>
  InstancePtr _getInstance(_Rx rel, _Args &&...args)
  {
    return this->__getInstance(std::move(rel), [&](_T *ptr)
                               { *ptr = new Element_NonPointer_Type(std::forward<_Args>(args)...); });
  }

  template <typename _T = Element_Type, typename _Rx,
            std::enable_if_t<std::is_pointer_v<_T>> * = nullptr>
  InstancePtr _getInstance(_Rx rel, _T value)
  {
    return this->__getInstance(std::move(rel), [&](Element_NonPointer_Type **ptr)
                               { *ptr = std::move(value); });
  }

public:
  Pool() = delete;
  ~Pool() = default;

  explicit Pool(const Container<Resource<E>>& init) noexcept
      : buffer(init)
  {
    this->_init();
  }

  static std::shared_ptr<Pool<Element_Type, Container>> createPool(const Container<Resource<E>>& init) noexcept
  {
    return std::make_shared<Pool<Element_Type, Container>>(init);
  }
  template <typename _Dx, typename... _Args,
            std::enable_if_t<std::is_invocable_v<_Dx, Element_Type &>> * = nullptr>
  InstancePtr getInstance(_Dx deleter, _Args &&...args)
  {
    struct _CRelease
    {
      std::shared_ptr<Pool<Element_Type, Container>> pool;
      _Dx del;
      explicit _CRelease(std::shared_ptr<Pool<Element_Type, Container>> pPool, _Dx _del)
          : pool(pPool), del(std::move(_del)) {}
      void operator()(Element_NonPointer_Type *pItem)
      {
        auto releaseResource = reinterpret_cast<Resource<Element_Type> *>(&pItem);
        del(releaseResource->live);
        pool->_Releasepool(releaseResource);
      }
    };
    return this->_getInstance(_CRelease(this->shared_from_this(), std::move(deleter)), args...);
  }

  template <typename _T = Element_Type, class... Args,
            std::enable_if_t<std::negation_v<std::is_pointer<_T>>> * = nullptr>
  InstancePtr getInstance(Args &&...args)
  {
    struct _Release
    {
      std::shared_ptr<Pool<_T, Container>> pool;
      explicit _Release(std::shared_ptr<Pool<_T, Container>> pPool)
          : pool(pPool) {}
      void operator()(std::remove_pointer_t<_T> *pItem)
      {
        auto releaseResource = reinterpret_cast<Resource<_T> *>(pItem);
        pool->_Releasepool(releaseResource);
      }
    };
    return this->_getInstance(_Release(this->shared_from_this()), args...);
  }
};
