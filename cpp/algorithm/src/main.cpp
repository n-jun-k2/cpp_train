#include <iostream>
#include <iterator>
#include <array>
#include <vector>
#include <memory>
#include <random>
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

template<class ST, class RT>
class eps_greedy {
  private:
    double EPS;
    std::random_device device;
    std::ranlux48 engine;
    std::unordered_map<ST, RT>& Q;
    std::uniform_real_distribution<> r_dist;
    std::uniform_int_distribution<> i_dist;
  public:
    eps_greedy(std::unordered_map<ST, RT>& q, const double eps, const int size)
    :Q(q), EPS(eps), device(), engine(device()), i_dist(0, size), r_dist(0.0, 1.0)
    {}

    ST operator()() {
      auto max_idx = std::distance(std::begin(Q), std::max_element(std::begin(Q), std::end(Q), [](const auto& a, const auto& b){return a.second > b.second;}));
      if (EPS > r_dist(engine))
        return i_dist(engine);
      else
        return static_cast<ST>(max_idx);
    }
};

template<class ST, class RT, template<class T, class A = std::allocator<T>>class CONTAINER>
class ucb_1 {
  private:
    double inf;
    unsigned long long N;
    std::unordered_map<ST, unsigned long long> NJ;
    std::unordered_map<ST, double> scores;
    std::unordered_map<ST, RT>& X;

  public:

    ucb_1(std::unordered_map<ST, RT>& q, const double inf, CONTAINER<ST> init)
    :X(q), inf(inf), N(0) {
      for(const auto& k : init) {
        NJ[k] = 0.0;
        scores[k] = 0.0;
      }
    }

    ST operator()() {
      const auto inc = 1.0;
      const auto is_inf = 0.0;
      const auto idx_size = X.size();

      /* select all options once. */
      if (N <= idx_size) return N++;

      ++N;
      for (auto idx = 0; idx < idx_size; ++idx)
        scores[idx] =  X[idx] + std::sqrt(2.0 * std::log(N) / (NJ[idx] + inf));

      auto max_itr = std::max_element(std::begin(scores), std::end(scores), [](const auto& a, const auto& b){return a.second > b.second;});
      auto max_idx = std::distance(std::begin(scores), max_itr);
      NJ[max_idx] += 1.0;

      return max_idx;
    }
};

/*多腕バンディット問題*/
template<int _SIZE, class _ENGIN_TYPE, class _STATE_TYPE, class _REAL_TYPE = float,
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

      this->output(std::cout);
    }

    _REAL_TYPE operator()(_STATE_TYPE i) {
      _ENGIN_TYPE engine(rand_device());
      return array_normal[i](engine);
    }

    std::ostream& output(std::ostream& os) {
      os << "=============" << std::endl;
      int i = 0;
      for (const auto& n : array_normal) {
        os << std::fixed << std::setprecision(2) <<"[" << i++ << "] mean=" << n.mean() << "," << "sigma=" << n.stddev() << std::endl;
      }
      os << "=============" << std::endl;
      return os;
    }
};

/*強化学習用フロー*/
struct ReinforcementLearning {
  template<class ST, class RT>
  static void For(
    std::function<ST()> choose_action,
    std::function<RT(ST)> choose_reward,
    std::function<bool(RT, ST, unsigned long long)> is_end) {
      unsigned long long idx = 0;
      ST action;
      RT reward;
      do {
        action = choose_action();
        reward = choose_reward(action);
      }while(!is_end(reward, action, idx++));
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
  { // 多腕バンディット問題のサンプル
    using REWARD_TYPE = double;
    using STATE_TYPE = int;

    constexpr int BANDIT_SIZE = 10;
    constexpr unsigned long long FOR_COUNT = 100;

    std::unordered_map<STATE_TYPE, REWARD_TYPE> Q;
    auto env = Bandit<BANDIT_SIZE, std::mt19937, STATE_TYPE, REWARD_TYPE>(0, 10, -1, 1);
    // auto policy = eps_greedy<STATE_TYPE, REWARD_TYPE>(Q, 0.8, BANDIT_SIZE);

    std::vector<STATE_TYPE> init_key(BANDIT_SIZE);
    std::iota(std::begin(init_key), std::end(init_key), 0);
    auto policy = ucb_1(Q, 0.01, init_key);

    ReinforcementLearning::For<STATE_TYPE, REWARD_TYPE>(
      [&](){return policy();},
      [&](STATE_TYPE T){return env(T);},
      [&](REWARD_TYPE r, STATE_TYPE s, unsigned long long i) {
        // update.
        const double INC = 1.0;
        auto mean = mean_1_14(Q[s], r, Q[s], Q[s] + INC);
        Q[s] = mean;
        return i > FOR_COUNT;
      });

    for(int i = 0; i < BANDIT_SIZE; ++i)
      std::cout << "[" << i << "]=" << Q[i] <<std::endl;
  }

  return 0;
}