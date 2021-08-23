#include <iostream>
#include <array>
#include <tuple>
#include <vector>

struct Chain{

  explicit Chain() = default;

  Chain(const Chain& other) {
    std::cout << "copy constructor" << std::endl;
  }

  Chain(Chain&& other) {
    std::cout << "move constructor" << std::endl;
  }

  Chain& func(int value) {
    std::cout << "value :" << value << std::endl;
    return *this;
  }
  Chain& func2(double value) {
    std::cout << "value :" << value << std::endl;
    return *this;
  }
  Chain& operator=(const Chain&) & {
    std::cout << "lvalue" << std::endl;
    return *this;
  }

  Chain& operator=(const Chain&) && {
    std::cout << "rvalue" << std::endl;
    return *this;
  }

};

struct Chain2{

  std::vector<int> buffer;

  explicit Chain2() :buffer(){}

  Chain2(const Chain2& other) {
    std::cout << "copy constructor" << std::endl;
  }

  Chain2(Chain2&& other) {
    std::cout << "move constructor" << std::endl;
  }

  Chain2& func(int value) {
    std::cout << "value :" << value << std::endl;
    return *this;
  }
  Chain2& func2(double value) {
    std::cout << "value :" << value << std::endl;
    return *this;
  }
  Chain2& operator=(const Chain2&) & {
    std::cout << "lvalue" << std::endl;
    return *this;
  }

  Chain2& operator=(const Chain2&) && {
    std::cout << "rvalue" << std::endl;
    return *this;
  }

};

void dammy(Chain& chain) {
  chain.func2(4.4);
}


int main() {

  Chain value;
  value = Chain().func(4).func2(3.4);

  dammy(Chain().func(5));

  auto value2 = Chain2().func(3).func2(3.3);


  Chain2 value3{};
  value3.func(3).func2(3.3);

  return 0;
}