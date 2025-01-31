#pragma once

#include <algorithm>
#include <golv/algorithm/move_ordering.hpp>
#include <golv/algorithm/unordered_table.hpp>
#include <golv/traits/game.hpp>
#include <iostream>
#include <limits>
#include <type_traits>
#include <unordered_map>

namespace golv {

/**
 * nega_max is the solver class using the negamax algorithm with alpha-beta pruning with or without memory and move
 * ordering. GameT satisfies concept Game. MoveOrderingT is a less-than ordering for GameT::move_type (typically an
 * integer, but can be different as long as either std::less is defined for the type or a user-defined ordering given.
 *  TableT satisfies concept TranspositionTable.
 */
template<Game GameT, typename MoveOrderingT = no_ordering, TranspositionTable<GameT> TableT = no_table<GameT>>
class nega_max
{
  public:
    using game_type = GameT;
    using value_type = typename game_type::value_type;
    using move_type = typename game_type::move_type;
    using move_ordering_type = MoveOrderingT;
    using table_type = TableT;

    constexpr static value_type min_value = std::numeric_limits<value_type>::lowest() + 1;
    constexpr static value_type max_value = std::numeric_limits<value_type>::max() - 1;

    nega_max(GameT game, MoveOrderingT move_ordering = no_ordering{}, TableT table = no_table<game_type>{})
      : game_(game)
      , move_ordering_(move_ordering)
      , table_(table)
    {
    }

    auto solve() -> value_type
    {
        return game_.is_max() ? _solve(min_value, max_value, 0) : -_solve(min_value, max_value, 0);
    }

    move_type best_move() const { return best_move_; }

  private:
    auto _solve(value_type a, value_type b, int depth) -> value_type
    {
        if (game_.is_terminal()) {
            return game_.is_max() ? game_.value() : -game_.value();
        }

        value_type value = min_value;
        value_type old_a = a;

        auto l = lookup_before<game_type, table_type>(table_, game_, a, b);
        if (l.first)
            return l.second;

        auto legal_actions = game_.legal_actions();

        if constexpr (with_ordering<move_ordering_type>::value) {
            std::sort(std::begin(legal_actions), std::end(legal_actions), move_ordering_);
        }

        best_move_ = move_type{};

        for (auto move : legal_actions) {
            game_.apply_action(move);
            value_type child_val = -_solve(-b, -a, depth + 1);
            game_.undo_action(move);

            if (child_val > value) {
                value = child_val;
                best_move_ = move;
            }

            a = std::max(a, value);
            if (a >= b) {
                break;
            }
        }

        _save_value(value, old_a, b);

        return value;
    }

    void _save_value(value_type value, value_type old_a, value_type b)
    {
        if constexpr (with_table<table_type>::value) {
            if (table_.is_memorable(game_)) {
                if (value <= old_a)
                    table_.set(game_.state(), lookup_value_type::upper_bound, value);
                else if (value >= b)
                    table_.set(game_.state(), lookup_value_type::lower_bound, value);
                else {
                    table_.set(game_.state(), lookup_value_type::exact, value);
                }
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
negamax(GameT g, LessT o = no_ordering{}, LookupT l = no_table<GameT>{})
{
    return nega_max(g, o, l).solve();
}

template<Game GameT, typename LessT = std::less<typename GameT::move_type>>
auto
negamax_with_memory(GameT g, LessT o = std::less<typename GameT::move_type>{})
{
    return nega_max(g, o, unordered_table<GameT>{}).solve();
}

} // namespace golv