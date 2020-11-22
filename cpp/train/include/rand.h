#include <algorithm>
#include <utility>
#include <array>
#include <numeric>


namespace rand {

  template<class T, size_t SIZE>
  std::array<T, SIZE>& shuffle(T value) {
    std::random_device seed_gen;
    std::mt19937 engine(seed_gen());

    std::array<T, SIZE> tempArray;
    std::iota(indexList.begin(), indexList.end(), value);
    std::shuffle(tempArray.begin(), tempArray.end(), engine);
    return tempArray;
  }

}