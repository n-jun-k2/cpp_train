/**
 * @file main.cpp
 * @author jun kawakami (http.kj.com@gmail.com)
 * @brief Calculation to find the discrete expected value of the dice.
 * @version 0.1
 * @date 2021-06-13
 *
 * @copyright Copyright (c) 2021
 *
 */

#include <iostream>
#include <array>
#include <numeric>
#include <algorithm>


constexpr int DICE_SIZE = 6;
std::array<float, DICE_SIZE> dice;
std::array<float, DICE_SIZE> dice_count {18,25,15,26,19,17,};
std::array<float, DICE_SIZE> dice_probability;

int main() {

  std::iota(std::begin(dice), std::end(dice), 1);

  const auto sum = std::accumulate(std::begin(dice_count), std::end(dice_count), 0.f);
  const auto mean = std::transform_reduce(std::begin(dice_count), std::end(dice_count), std::begin(dice), 0.f) / sum;

  std::cout << "sum = " << sum << std::endl; // 120
  std::cout << "mean = " << mean << std::endl; // 3.45

  // create dice probability
  std::generate(std::begin(dice_probability), std::end(dice_probability), [](){ return 1.f / DICE_SIZE; });

  const auto Expectation = std::transform_reduce(std::begin(dice), std::end(dice), std::begin(dice_probability), 0.f);
  std::cout << "Expectation = " << Expectation << std::endl; // 3.5

  return 0;
}