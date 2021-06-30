#include <iostream>
#include <array>
#include <vector>
#include <memory_resource>


template<std::size_t SECTOR = 512>
class original_resource: public std::pmr::memory_resource {

  std::array<std::byte, SECTOR> m_buffer {};
  std::byte* m_position {};

public:
  original_resource() : m_position(std::begin(m_buffer)) {}
  original_resource(original_resource const&) = delete;
  original_resource& operator=(original_resource const&) = delete;
  virtual ~original_resource() = default;

protected:

  void* do_allocate(std::size_t bytes, [[maybe_unused]] std::size_t alignment) override {
    std::cout << "do_allocate:" << bytes << "," << alignment << std::endl;
    if(bytes > remaining()) throw std::bad_alloc();

    auto memory = m_position;
    m_position += bytes;

    return memory;
  }

  void do_deallocate(void* p, std::size_t bytes, [[maybe_unused]] std::size_t alignment) override {
    std::cout << "do_deallocate : " << p << ":" << bytes << "," << alignment << std::endl;
    if (!pointer_in_buffer(p)) return;
    if (auto pb = static_cast<std::byte*>(p); (pb + bytes) == m_position) {
      m_position = pb;
    }
  }

  bool do_is_equal(const std::pmr::memory_resource& other) const noexcept override {
    return this == &other;
  }

private:
  std::size_t remaining() const {
    return std::end(m_buffer) - m_position;
  }

  bool pointer_in_buffer(void* ptr) const {
    return std::begin(m_buffer) <= ptr && ptr < std::end(m_buffer);
  }
};

namespace std::pmr {
  template <typename T>
  using vector = std::vector<T, std::pmr::polymorphic_allocator<T>>;
}

struct Sample {
  int a;
  char b = 10;
  char c = 10;
  char d = 10;
  Sample(int _a, char _b):a(_a),b(_b){}
};

std::ostream& operator<<(std::ostream& os, const Sample& dt)
{
    os << dt.a << '/' << dt.b << '/' << dt.c << '/' << dt.d;
    return os;
}


template<typename T, std::size_t N, typename... _Args>
void test_code(_Args&& ...args) {
  std::cout << "size : " << sizeof(T) << std::endl;
  original_resource<N> resource;
  std::pmr::vector<T> vec({args...}, &resource);
  for (const auto& o : vec) {
    std::cout << o << std::endl;
  }
}

int main() {

  test_code<uint32_t, 16>(1, 2, 3, 4);
  test_code<Sample, 16>((1, 1), (2, 2), (3, 3), (4, 4));

  return 0;
}