#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>
#include <string>

struct MyType {
  int value;
};

MyType operator""int(unsigned long long int value){
  return MyType{static_cast<int>(value)};
}

struct MyFloat {
  float value;
};

MyFloat operator""float(long double value) {
  return MyFloat{static_cast<float>(value)};
}

struct MyString {
  std::string msg;
};

MyString operator""string(const char* str, std::size_t length)
{
  return MyString{std::string(str, length)};
}

int main() {

  auto x = 42int;

  std::cout << x.value << std::endl;

  auto y = 0.13float;

  std::cout << y.value << std::endl;

  auto z = "AbCDDefg"string;

  std::cout << z.msg << std::endl;

  return 0;
}