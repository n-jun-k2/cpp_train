#include <iostream>
#include <tuple>

int main() {
  auto item = std::make_tuple('\0', 'A', 'B');
  char *pItem = reinterpret_cast<char*>(&item);
  std::cout << sizeof(item) << std::endl;
  std::cout << pItem << std::endl;
  return 0;
}