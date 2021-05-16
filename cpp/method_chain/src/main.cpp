#include <iostream>
#include <array>
#include <tuple>

struct Chain{

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

void dammy(Chain& chain) {
  chain.func2(4.4);
}


int main() {

  Chain value;
  value = Chain().func(4).func2(3.4);

  dammy(Chain().func(5));

  return 0;
}