#include <iostream>
#include <iterator>
#include <array>
#include <vector>
#include <memory>
#include <random>
#include <map>
#include <unordered_map>
#include <numeric>
#include <algorithm>
#include <functional>
#include <type_traits>
#include <iomanip>
#include <cmath>
#include <cfloat>

template<class T>
float mean_1_11(std::vector<T> data) {
  return std::accumulate(std::begin(data), std::end(data), 0.f) / data.size();
}

double mean_1_14(const double _X, const double x, const double t, const double N = 0) {
  return _X + (1.0 / (N - t)) * (x - _X);
}

double mean_1_15(const double _X, const double x, const double t, const double N = 0) {
  const double current = N - t;
  return ((current - 1.0) / current) * _X + (1.0 / current) * x;
}

double mean_1_20(const double _X, const double a, const double v) {
  return _X + (a * (v - _X));
}

template<class AT>
class eps_greedy {
  public:
    using MAX_FUNC = std::function<AT()>;
    using RANDOM_FUNC = std::function<AT(const double)>;
  private:
    double EPS;
    std::random_device device;
    std::ranlux48 engine;
    std::uniform_real_distribution<> r_dist;
    MAX_FUNC max;
    RANDOM_FUNC random;
  public:
    eps_greedy(const double eps, MAX_FUNC get_max, RANDOM_FUNC get_random)
    : EPS(eps), device(), engine(device()), r_dist(0.0, 1.0), max(get_max), random(get_random){}

    AT operator()() {
      if (EPS > r_dist(engine))
        return random(r_dist(engine));
      else
        return max();
    }
};

template<class ST>
class ucb_1 {
  public:
    using real_type = double;
    using integer_type = unsigned int;
    using value_func = std::function<real_type(integer_type)>;
    using is_order_func = std::function<bool(integer_type)>;
    using order_action_func = std::function<ST(integer_type)>;
  private:
    real_type inf;
    integer_type N;
    std::unordered_map<ST, integer_type> NJ;
    std::unordered_map<ST, real_type> scores;

    value_func get_value;
    is_order_func is_order;
    order_action_func order_action;

  public:

    ucb_1(const real_type inf, value_func get_value, is_order_func is_order, order_action_func order_action)
    :inf(inf), get_value(get_value), is_order(is_order), order_action(order_action), N(0) {}

    ST operator()() {

      /* select all options once. */
      if (is_order(N)) return order_action(N++);

      for (auto idx = 0; is_order(idx); ++idx)
        scores[idx] =  get_value(idx) + std::sqrt(2.0 * std::log(N) / (NJ[idx] + inf));

      auto max_itr = std::max_element(std::begin(scores), std::end(scores), [](const auto& a, const auto& b){return a.second > b.second;});
      auto max_idx = std::distance(std::begin(scores), max_itr);
      NJ[max_idx] += 1.0;

      return order_action(max_idx);
    }
};


/*多腕バンディット問題*/
template<int _SIZE, class _ENGIN_TYPE, class _STATE_TYPE=int, class _REAL_TYPE = float,
  std::enable_if_t<std::is_constructible_v<_ENGIN_TYPE, std::random_device::result_type>>* = nullptr>
class Bandit {
  private:
    std::random_device rand_device;
    std::vector<std::normal_distribution<_REAL_TYPE>> array_normal;

  public:
    explicit Bandit(_REAL_TYPE _mean_min, _REAL_TYPE _mean_max, _REAL_TYPE _dist_min, _REAL_TYPE _dist_max) {
      _ENGIN_TYPE engine(rand_device());
      std::uniform_real_distribution<_REAL_TYPE> _mean(_mean_min, _mean_max);
      std::uniform_real_distribution<_REAL_TYPE> _dist(_dist_min, _dist_max);
      std::generate_n(std::back_inserter(array_normal), _SIZE,
        [&](){
          return std::normal_distribution<_REAL_TYPE>(_mean(engine), _dist(engine));
        });

      this->output_state(std::cout);
    }

    _REAL_TYPE operator()(_STATE_TYPE i) {
      _ENGIN_TYPE engine(rand_device());
      return array_normal[i](engine);
    }

    void output_state(std::ostream& os) {
      os << "=============" << std::endl;
      int i = 0;
      for (const auto& n : array_normal) {
        os << std::fixed << std::setprecision(2) <<"[" << i++ << "] mean=" << n.mean() << "," << "sigma=" << n.stddev() << std::endl;
      }
      os << "=============" << std::endl;
    }
};

/*強化学習用フロー*/
struct ReinforcementLearning {
  using INDEX = unsigned long long;
  template<class AT, class RT>
  static void For(
    const std::function<AT(INDEX, RT)> choose_action,
    const std::function<RT(INDEX, AT)> choose_reward,
    const std::function<bool(INDEX, RT, AT)> is_end) {
    INDEX idx = 0;
    AT action;
    RT reward;
    do {
      action = choose_action(idx, reward);
      reward = choose_reward(idx, action);
    } while(!is_end(idx++, reward, action));
  }
};

int main() {
  { // 逐次平均のサンプル
    constexpr int VECTOR_SIZE = 10;

    std::random_device rand_device;
    std::ranlux48 rand_engine_ranlux48(rand_device());
    std::uniform_int_distribution<> uniform(1, 10);

    std::vector<int> X;

    std::generate_n(std::back_inserter(X), VECTOR_SIZE, [&](){return uniform(rand_engine_ranlux48);});
    std::cout << "vector:";
    std::copy(std::begin(X), std::end(X), std::ostream_iterator<int>(std::cout, ","));
    std::cout << std::endl;

    {
      float V = 0;
      int t = 0;
      for(const auto x : X) V = mean_1_14(V, x, --t); // --t は過去に対する平均を行う為デクリメントを行っている
      std::cout << "past mean:" << V << std::endl;
    }

    {
      float V = 0;
      int t = 0;
      for(const auto x : X) V = mean_1_15(V, x, --t); // --t は過去に対する平均を行う為デクリメントを行っている
      std::cout << "past mean:" << V << std::endl;
    }

    {
      const auto mean = mean_1_11(X);
      std::cout << "mean:" << mean << std::endl;
    }
  }
  { // 多腕バ ンディット問題のサンプル
    using REWARD_TYPE = double;
    using ACTION_TYPE = int;
    using VALUE_FUNC_TYPE = std::unordered_map<ACTION_TYPE, REWARD_TYPE>;

    constexpr int BANDIT_SIZE = 10;
    constexpr unsigned long long FOR_COUNT = 100;
    constexpr double EPS = 0.3;

    VALUE_FUNC_TYPE Q;
    auto env = Bandit<BANDIT_SIZE, std::mt19937, ACTION_TYPE, REWARD_TYPE>(0, BANDIT_SIZE, -1, 1);
    // auto policy = eps_greedy<int>{
    //   EPS,
    //   [&]() -> int{
    //     auto max_itr = std::max_element(std::begin(Q), std::end(Q),
    //       [](const auto& a, const auto& b) {return a.second > b.second;});
    //     int max_idx = std::distance(std::begin(Q), max_itr);
    //     return max_idx;
    //   },
    //   [&](const double r) -> int{
    //     auto idx = static_cast<int>((BANDIT_SIZE) * r);
    //     return idx;
    //   }};
    auto policy = ucb_1<ACTION_TYPE>(
      0.01,
      [&](unsigned int a){
        return Q[a];
      },
      [BANDIT_SIZE](unsigned int n){
        return n < BANDIT_SIZE;
      },
      [&](unsigned int n){
        return n;
      });

    ReinforcementLearning::For<ACTION_TYPE, REWARD_TYPE>(
      [&](unsigned long long i, REWARD_TYPE r){return policy();},
      [&](unsigned long long i, ACTION_TYPE a){return env(a);},
      [&](unsigned long long i, REWARD_TYPE r, ACTION_TYPE a) {
        // update.
        const double INC = 1.0;
        auto mean = mean_1_14(Q[a], r, Q[a], Q[a] + INC);
        Q[a] = mean;
        return i > FOR_COUNT;
      });

    for(int i = 0; i < BANDIT_SIZE; ++i)
      std::cout << "[" << i << "]=" << Q[i] <<std::endl;
  }

  return 0;
}