#pragma once

#include <algorithm>
#include <golv/algorithm/move_ordering.hpp>
#include <golv/algorithm/unordered_table.hpp>
#include <golv/traits/game.hpp>
#include <golv/util/logging.hpp>
#include <iostream>
#include <limits>
#include <type_traits>
#include <unordered_map>

namespace golv {

/**
 * alpha_beta is the solver class using the alpha-beta pruning algorithm with or without memory and move ordering.
 *  GameT satisfies concept Game.
 *  MoveOrderingT is a less-than ordering for GameT::move_type (typically an integer, but can be different as long as
 * either std::less is defined for the type or a user-defined ordering given.
 *  TableT satisfies concept TranspositionTable.
 */
template<Game GameT, typename MoveOrderingT = no_ordering, TranspositionTable<GameT> TableT = no_table<GameT>>
class alpha_beta
{
  public:
    using game_type = GameT;
    using value_type = typename game_type::value_type;
    using move_type = typename game_type::move_type;
    using move_ordering_type = MoveOrderingT;
    using table_type = TableT;

    constexpr static value_type min_value = std::numeric_limits<value_type>::lowest() / 2;
    constexpr static value_type max_value = std::numeric_limits<value_type>::max() / 2;

    alpha_beta(GameT game, MoveOrderingT move_ordering = no_ordering{}, TableT table = no_table<game_type>{})
      : game_(game)
      , move_ordering_(move_ordering)
      , table_(table)
    {
    }

    auto solve() -> value_type
    {
        best_move_ = move_type{};
        return _solve(min_value, max_value, 0);
    }

    auto mws_solve(value_type b) -> value_type { return _solve(b - 1, b, 0); }

    move_type best_move() const { return best_move_; }

    TableT const& get_table() const { return table_; }

  private:
    auto _solve(value_type a, value_type b, int depth) -> value_type
    {
        if (game_.is_terminal()) {
          return 0;
        }

        value_type opt = game_.is_max() ? min_value : max_value;
        value_type old_a = a, old_b = b;

        auto l = lookup_before<game_type, table_type>(table_, game_, a, b);
        if (l.first)
            return l.second;

        auto legal_actions = game_.legal_actions();

        if constexpr (with_ordering<move_ordering_type>::value) {
            std::sort(std::begin(legal_actions), std::end(legal_actions), move_ordering_);
        }

        for (auto const& move : legal_actions) {
            game_.apply_action(move);
            value_type move_value = game_.value();
            value_type value = move_value + _solve(a - move_value, b - move_value, depth + 1);
            game_.undo_action(move);

            if (game_.is_max()) {
              if (depth == 0 && value > opt) {
                best_move_ = move;
              }
                opt = std::max(value, opt);
                a = std::max(a, value);
            } else {
              if (depth == 0 && value < opt) {
                best_move_ = move;
              }
                opt = std::min(opt, value);
                b = std::min(b, value);
            }
            if (a >= b) {
                if (game_.is_max()) {
                  _save_value(lookup_value_type::lower_bound, value);
                  return a;
                } else {
                  _save_value(lookup_value_type::upper_bound, value);
                  return b;
                }
            }
        }

        if (opt > old_a && opt < old_b) {
          _save_value(lookup_value_type::exact, opt);
        }

        return game_.is_max() ? a : b;
    }

    void _save_value(lookup_value_type type, value_type value)
    {
        if constexpr (with_table<table_type>::value) {
          if (table_.is_memorable(game_)) {
            table_.set(game_.state(), type, value);
          }
        }
    }

    game_type game_;
    move_ordering_type move_ordering_;
    table_type table_;
    move_type best_move_;
};

template<Game GameT, typename LessT = no_ordering, typename LookupT = no_table<GameT>>
auto
alphabeta(GameT g, LessT o = no_ordering{}, LookupT l = no_table<GameT>{})
{
    alpha_beta ab(g, o, l);
    auto solution = ab.solve();
    return std::make_pair(solution, ab.best_move());
}

template<Game GameT, typename LessT = std::less<typename GameT::move_type>>
auto
alphabeta_with_memory(GameT g, LessT o = std::less<typename GameT::move_type>{})
{
    alpha_beta ab(g, o, unordered_table<GameT>{});
    auto solution = ab.solve();
    GOLV_LOG_TRACE("Table = " << ab.get_table());
    return std::make_pair(solution, ab.best_move());
}

} // namespace golv