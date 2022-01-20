#include <iostream>
#include <random>
#include <type_traits>
#include <unordered_map>
#include <functional>

double mean_1_14(const double _X, const double x, const long t, const long N = 0) {
  return _X + (1.0 / (N - t)) * (x - _X);
}

double mean_1_15(const double _X, const double x, const long t, const long N = 0) {
  const double current = N - t;
  return ((current - 1.0f) / current) * _X + (1.0f / current) * x;
}

template<class RESULT_TYPE, class _ENGIN_TYPE>
std::enable_if_t<std::is_constructible_v<_ENGIN_TYPE, std::random_device::result_type>>
print_random_mean(const unsigned COUNT, std::function<RESULT_TYPE(_ENGIN_TYPE&)> dist) {

  constexpr double INC = 1.0;
  std::random_device rand_device;
  _ENGIN_TYPE engine(rand_device());
  std::unordered_map<unsigned, double> avg;

  for (unsigned i = 0; i < COUNT; ++i) {
    unsigned t = static_cast<unsigned>(dist(engine)) - 1;
    // avg[t] += INC / (COUNT - (avg[t] + INC));
    // avg[t] = mean_1_14(avg[t], avg[t] + INC, avg[t] + INC, COUNT);
    avg[t] = mean_1_15(avg[t], avg[t] + INC, avg[t] + INC, COUNT);
  }

  for(auto& item : avg) std::cout << "[" << item.first << "]=" << item.second <<std::endl;
}


int main() {

  double interval[] = {1,   2,   5,   6};
  double weights[] =  {  .90, .06, .04};
  std::piecewise_constant_distribution<> dist(std::begin(interval),
                                              std::end(interval),
                                              std::begin(weights));

  print_random_mean<double, std::mt19937>(100000, [&dist](std::mt19937& e) -> double {return dist(e);});

  return 0;
}