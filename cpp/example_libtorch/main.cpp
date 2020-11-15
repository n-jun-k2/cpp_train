#include <iostream>

#include "../utils/metaheuristic.h"


int main(){

  auto option = metaheuristic::getAvailableOption();
  std::cout << "HELLO WORLD" << std::endl;

  std::cout << option << std::endl;
  auto randTensor = torch::randn({3, 3}, option);
  std::cout << randTensor << std::endl;

  return 0;
}
