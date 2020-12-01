#include <algorithm>
#include <utility>
#include <array>
#include <numeric>


namespace utils{

  /**
   * @brief 乱数配列を作成する(重複なし)
   *
   * @tparam T
   * @tparam SIZE
   * @param value
   * @return std::array<T, SIZE>&
   */
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