#pragma once

#include <algorithm>
#include <golv/algorithm/alphabeta.hpp>
#include <golv/traits/game.hpp>
#include <iostream>
#include <limits>
#include <type_traits>
#include <unordered_map>

#include "transposition_table.hpp"

namespace golv {

template <Game GameT, typename MoveOrderingT = no_ordering,
          TranspositionTable<GameT> TableT = no_table<GameT>>
class mtd_f {
public:
  using game_type = GameT;
  using value_type = typename game_type::value_type;
  using move_type = typename game_type::move_type;

  constexpr static value_type min_value =
      std::numeric_limits<value_type>::lowest();
  constexpr static value_type max_value =
      std::numeric_limits<value_type>::max();

private:
  game_type game_;

public:
  mtd_f(GameT game) : game_(game) {}

  value_type solve(value_type first_guess, value_type lower_bound = min_value, value_type upper_bound = max_value) {
    value_type g = first_guess;

    unordered_table<game_type> table;
    alpha_beta solver(game_, std::less<move_type>{}, table);
    while (lower_bound < upper_bound) {
      GOLV_LOG_DEBUG("lower_bound = " << lower_bound << ", upper_bound = " << upper_bound);
      value_type beta = g > lower_bound + 1 ? g : (lower_bound + 1);
      g = solver.mws_solve(beta);
      if (g < beta)
        upper_bound = g;
      else
        lower_bound = g;
      GOLV_LOG_DEBUG("table size = " << solver.get_table().size());
    }

    return g;
  }
};

} // namespace golv