#include <iostream>
#include <string>
#include <vector>
#include <algorithm>


template <  typename RAW_TYPE, typename TYPE, typename SIZE = int, template< typename E, typename Allocator = std::allocator<E> > class Container = std::vector>
void containerToCPtr(SIZE& ref_size, RAW_TYPE** ptr_value, Container<TYPE>& container) {
  ref_size = static_cast<SIZE>(container.size());
  *ptr_value = nullptr;
  if (!container.empty()){
    *ptr_value = container.data();
  }
}


template< template< typename E, typename Allocator = std::allocator<E> > class Container = std::vector>
void stringListToCPtrList(const Container<std::string>& src, Container<const char*>& dst) {
  dst.reserve(src.size());
  std::transform(std::begin(src), std::end(src), std::back_inserter(dst), [](const std::string& s){
    return s.c_str();
  });
}

void func(const std::string& name) {
  std::cout << "string :" << name << std::endl;
}

void func(const char* name) {
  std::cout << "raw :" << name << std::endl;
}

int main() {

  const char* const* ppNames;
  int nameSize;
  std::vector<const char*> rawString;
  std::vector<std::string> ppString {
    "abc",
    "jun",
    "kawakami",
  };

  stringListToCPtrList(ppString, rawString);
  containerToCPtr(nameSize, &ppNames, rawString);

  for (auto& data : ppString) {
    std::cout << "data : " << data << std::endl;
  }

  for (auto& raw : rawString) {
    std::cout << "raw data:" << raw << std::endl;
  }

  std::cout << nameSize << std::endl;
  for(auto i = 0; i < nameSize; ++i) {
    std::cout << ppNames[i] << std::endl;
  }

  auto p = std::string("name");
  func(p);

  func("ppp");
  func(std::string("name"));

  return 0;
}