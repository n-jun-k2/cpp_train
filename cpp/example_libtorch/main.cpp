#include <iostream>
#include <torch/torch.h>

const auto CPU_OPTIONS = torch::TensorOptions()
									.dtype(torch::kFloat32)
									.layout(torch::kStrided)
									.device(torch::kCPU)
									.requires_grad(false);

int main(){

  std::cout << "HELLO WORLD" << std::endl;

  auto randTensor = torch::randn({3, 3}, CPU_OPTIONS);
  std::cout << randTensor << std::endl;

  return 0;
}
