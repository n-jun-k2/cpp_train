#include <iostream>
#include <vector>


int main() {

  const int* ptr = new int[3];

  {
    auto vec = std::vector<int>{1, 2, 3};

    auto pInt = std::remove_const_t<int*>(ptr);
    std::copy(vec.begin(), vec.end(), pInt);

    std::cout << "local[0]" << ptr[0] << std::endl;
    std::cout << "local[1]" << ptr[1] << std::endl;
    std::cout << "local[2]" << ptr[2] << std::endl;

  }

  std::cout << "[0]" << ptr[0] << std::endl;
  std::cout << "[1]" << ptr[1] << std::endl;
  std::cout << "[2]" << ptr[2] << std::endl;

  delete[] ptr;
  // dead...
  std::cout << "[0]" << ptr[0] << std::endl;
  std::cout << "[1]" << ptr[1] << std::endl;
  std::cout << "[2]" << ptr[2] << std::endl;

  return 0;
}