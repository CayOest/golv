#pragma once

#include <golv/traits/game.hpp>
#include <golv/util/logging.hpp>
#include <random>

namespace golv {

template <Game GameT>
class cfr {
  public:
    using game_type = GameT;
    using value_type = double;
    using strategy_type = std::vector<double>;
    using information_set_type = typename game_type::information_set_type;

    constexpr static double threshold = 1e-6;

    struct node {
      size_t N = 3;
      strategy_type regret_sum = std::vector<double>(N, 0.0);
      strategy_type strategy_sum = std::vector<double>(N, 0.0);

      strategy_type strategy() const {
        auto strat = regret_sum;
        std::transform(std::begin(strat), std::end(strat), std::begin(strat), [](auto s) { return std::max(s, 0.0); });
        auto sum = std::accumulate(std::begin(strat), std::end(strat), 0.0);
        if (sum > threshold) {
          std::transform(std::begin(strat), std::end(strat), std::begin(strat), [sum](auto s) { return s / sum; });
          return strat;
        } else
          return _default_strategy();
      }

      strategy_type avg_strategy() const {
        auto strat = strategy_sum;
        auto sum = std::accumulate(std::begin(strategy_sum), std::end(strategy_sum), 0.0);
        if (sum > threshold) {
          std::transform(std::begin(strat), std::end(strat), std::begin(strat), [sum](auto s) { return s / sum; });
          return strat;
        } else
          return _default_strategy();
      }

      strategy_type _default_strategy() const { return std::vector<double>(N, 1.0 / N); }
    };

    using map_type = std::map<information_set_type, node>;
    using move_type = typename game_type::move_type;

    constexpr static int num_players = 2;

    cfr(GameT game) : game_(game) {}

    auto solve(int iterations = 1000) -> value_type {
      strategy_type util(num_players, 0.0);
      for (int i = 0; i < iterations; ++i) {
        GOLV_LOG_TRACE("Iteration = " << i);
        for (int j = 0; j < num_players; ++j) {
          game_.set_max(j);
          util[j] += _solve();
        }
        GOLV_LOG_TRACE("/Iteration");
      }
      for (int j = 0; j < num_players; ++j) {
        util[j] /= iterations;
      }
      return util[0];
    }

    auto const& map() const { return map_; }

   private:
    auto _solve(int depth = 0) -> value_type {
      GOLV_LOG_INFO(depth);
      if (game_.is_terminal()) return game_.value();
      if (game_.is_chance_node()) {
        // do stuff
        return 0.0;
      }
      if (game_.is_max()) {  // traversing player -> iterate over all actions
        return _traverse_max(depth);

      } else {  // non-traversing player -> choose action at random according to the strategy
        return _traverse_non_max(depth);
      }
      return 0.0;
    }

    auto _traverse_non_max(int depth) -> value_type {
      auto strat = map_[game_.state()].strategy();
      auto action = _choose_action();
      game_.apply_action(action);
      auto util = _solve();
      game_.undo_action(action);
      for (int i = 0; i < 3; ++i) {
        map_[game_.state()].strategy_sum[i] += strat[i];
      }
      return util;
    }

    auto _traverse_max(int depth) -> value_type {
      strategy_type util(3, 0.0);
      double node_util = 0.0;
      auto info_set = game_.state();
      auto strat = map_[info_set].strategy();

      for (size_t i = 0; i < 3; ++i) {
        auto action = game_.legal_actions()[i];
        game_.apply_action(action);
        util[i] = _solve(depth + 1);
        game_.undo_action(action);
        node_util += strat[i] * util[i];
      }

      for (size_t i = 0; i < 3; ++i) {
        double regret = util[i] - node_util;
        map_[info_set].regret_sum[i] += regret;
      }

      return node_util;
    }

    game_type game_;
    map_type map_;

    auto _default_strat() -> strategy_type {
      auto const _actions = game_.legal_actions();
      strategy_type default_strat;
      for (auto& s : default_strat) {
        s = 1.0 / _actions.size();
      }
      return default_strat;
    }

    auto _accumulated_strat(auto&& strat) -> strategy_type {
      strategy_type acc;
      std::partial_sum(std::begin(strat), std::end(strat), std::back_inserter(acc));
      return acc;
    }

    auto _rnd() -> double {
      static std::random_device rd_;   // Will be used to obtain a seed for the random number engine
      static std::mt19937 gen(rd_());  // Standard mersenne_twister_engine seeded with rd()
      static std::uniform_real_distribution<> dis(0.0, 1.0);
      return dis(gen);
    }

    auto _choose_action() -> move_type {
      GOLV_LOG_TRACE("_choose_action");
      if (!game_.is_max()) {
        information_set_type info_set = game_.state();
        GOLV_LOG_TRACE("info_set = " << info_set);
        auto it = map_.find(info_set);
        if (it == map_.end()) {
          map_[info_set] = node();
        }
        auto const& strat = map_[info_set].strategy();
        auto acc = _accumulated_strat(strat);
        auto const& legal = game_.legal_actions();
        auto choice = _rnd();
        GOLV_LOG_TRACE("choice = " << choice);
        auto it2 = std::find_if(std::begin(acc), std::end(acc), [choice](auto s) { return choice < s; });
        return *(std::begin(legal) + std::distance(std::begin(acc), it2));
      }
      GOLV_LOG_TRACE("/_choose_action");
    }
};

}  // namespace golv