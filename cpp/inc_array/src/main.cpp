#include <iostream>
#include <array>
#include <vector>
#include <numeric>
#include <algorithm>

template<template<typename T, typename Allocator = std::allocator<T>> class Container>
void func(Container<int>& pArray) {
  for (auto& p : pArray) {
    std::cout << p << std::endl;
  }
}

template<uint64_t S>
void func(std::array<int, S>& pArray) {
  for (auto& p : pArray) {
    std::cout << p << std::endl;
  }
}

int main() {
  auto a1 = std::array<int, 10>{};
  auto b1 = std::vector<int>(5);

  std::iota(std::begin(a1), std::end(a1), 1);
  std::iota(std::begin(b1), std::end(b1), 1);

  func(a1);
  func(b1);

  return 0;
}