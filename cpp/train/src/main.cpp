#include "../include/mha.h"
#include <iostream>



int main() {

  auto options = mha::getAvailableOption();

  auto A = torch::tensor({1, 2, 3, 4}, options).reshape({2, 2});
  auto x = torch::tensor({0.5, 0.8}, options);
  auto y = torch::tensor({5, 10}, options);

  std::cout << A << std::endl;

  std::cout << A * x << std::endl;

  std::cout << A + y << std::endl;


  auto matrix = mha::rand(
    torch::ones({10}, options) * 5,
    torch::ones({10}, options) * 2,
    {10, 10},
    options);
  std::cout << matrix << std::endl;

  std::cout << matrix.where(matrix > 3.5, torch::zeros(matrix.sizes(), options)) << std::endl;

  auto perm = torch::randperm(10, options);
  std::cout << perm << std::endl;

  // 条件に会う要素だけが1それ以外は0の行列
  auto condition = matrix > torch::Scalar(3.5);
  // 0,1の行列を作成
  std::cout << condition << std::endl;
  // 0以外の要素を取得
  std::cout << matrix.masked_select(matrix[0] > 3.5) << std::endl;
  // 0以外のインデックスを取得
  auto indexs = condition.nonzero();
  std::cout << condition.nonzero() << std::endl;

  auto tensor = torch::tensor({
    0, 1, 3, 0,
    0, 0, 0, 0,
    0, 0, 0, 5,
    0, 0, 0, 0,
  }).reshape({4, 4});
  auto indexs_list = mha::nonzero(tensor);
  for(const auto& row : indexs_list){
    if(row.size(0) <= 0)continue;
    std::cout << row << std::endl;
  }

  std::cout << condition.all() << std::endl;

  return 0;
}