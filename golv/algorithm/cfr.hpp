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
      game_type::move_range actions;
      strategy_type regret_sum;
      strategy_type strategy_sum;

      node(game_type::move_range const& _actions) : actions(_actions) {
        regret_sum = std::vector<double>(actions.size(), 0.0);
        strategy_sum = std::vector<double>(actions.size(), 0.0);
      }

      size_t size() const { return actions.size(); }

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

      strategy_type _default_strategy() const { return std::vector<double>(size(), 1.0 / size()); }
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
          GOLV_LOG_TRACE("Player = " << j);
          game_ = game_type();
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
      GOLV_LOG_INFO("depth = " << depth);
      if (game_.is_terminal()) return game_.value();
      if (game_.is_chance_node()) {
        return _handle_chance_node(depth);
      }
      if (game_.is_max()) {  // traversing player -> iterate over all actions
        return _traverse_max(depth);

      } else {  // non-traversing player -> choose action at random according to the strategy
        return _traverse_non_max(depth);
      }
      return 0.0;
    }

    auto _handle_chance_node(int depth) -> value_type {
      GOLV_LOG_TRACE("_handle_chance_node");
      game_.handle_chance_node();
      return _solve(depth + 1);
    }

    auto _get_node() {
      auto info_set = game_.state();
      auto it = map_.find(info_set);
      auto legal = game_.legal_actions();
      if (it == map_.end()) {
        it = map_.insert({info_set, node(legal)}).first;
      }
      return it;
    }

    auto _traverse_non_max(int depth) -> value_type {
      GOLV_LOG_TRACE("_traverse_non_max");
      auto it = _get_node();
      auto strat = it->second.strategy();
      auto action = _choose_action();  // choose action at random
      game_.apply_action(action);
      auto util = _solve(depth + 1);
      game_.undo_action(action);
      for (size_t i = 0; i < it->second.size(); ++i) {
        it->second.strategy_sum[i] += strat[i];
      }
      return util;
    }

    auto _traverse_max(int depth) -> value_type {
      GOLV_LOG_TRACE("_traverse_max");
      auto legal = game_.legal_actions();
      strategy_type util(legal.size(), 0.0);
      double node_util = 0.0;
      auto it = _get_node();

      auto strat = it->second.strategy();
      for (size_t i = 0; i < legal.size(); ++i) {
        auto action = legal[i];
        game_.apply_action(action);
        util[i] = _solve(depth + 1);
        game_.undo_action(action);
        node_util += strat[i] * util[i];
      }

      for (size_t i = 0; i < it->second.size(); ++i) {
        double regret = util[i] - node_util;
        it->second.regret_sum[i] += regret;
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
        auto it = _get_node();
        information_set_type info_set = game_.state();
        GOLV_LOG_TRACE("info_set = " << info_set);
        auto const& legal = game_.legal_actions();
        auto const& strat = it->second.strategy();
        auto acc = _accumulated_strat(strat);
        auto choice = _rnd();
        GOLV_LOG_TRACE("choice = " << choice);
        auto it2 = std::find_if(std::begin(acc), std::end(acc), [choice](auto s) { return choice < s; });
        return *(std::begin(legal) + std::distance(std::begin(acc), it2));
      } else {
        throw std::domain_error("Error: Can only choose action for non-max player.");
      }
      GOLV_LOG_TRACE("/_choose_action");
    }
};

}  // namespace golv