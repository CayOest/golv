#pragma once

#include <golv/traits/game.hpp>
#include <golv/algorithms/mws_unordered_table.hpp>
#include <golv/algorithms/move_ordering.hpp>
#include <golv/util/logging.hpp>

#include <algorithm>

template <class T>
struct opp_value_wrapper : public T {
  template <class U>
  static auto check(U const& t) -> decltype(t.opp_value(), std::true_type());

  static auto check(...) -> decltype(std::false_type());
};

template <class T>
struct has_opp_value : decltype(opp_value_wrapper<T>::check(std::declval<opp_value_wrapper<T>>())){};

template <class T>
struct value_bounds_wrapper : public T
{
  template <class U>
  static auto check(U const& t) -> decltype(U::value_bounds, std::true_type());

  static auto check(...) -> decltype(std::false_type());
};

template <class T>
struct has_value_bounds : decltype(value_bounds_wrapper<T>::check(
                              std::declval<value_bounds_wrapper<T>>())){};

namespace golv {
template <Game GameT, TranspositionTable<GameT> TableT = no_table<GameT>,
          typename MoveOrderingT = no_ordering>
class minimal_window_search {
 public:
  using game_type = GameT;
  using value_type = typename game_type::value_type;
  using move_type = typename game_type::move_type;
  using move_ordering_type = MoveOrderingT;
  using table_type = TableT;

  constexpr static value_type min_value = std::numeric_limits<value_type>::lowest() / 2;
  constexpr static value_type max_value = std::numeric_limits<value_type>::max() / 2;

  minimal_window_search(GameT game, TableT table = no_table<game_type>{},
                        MoveOrderingT move_ordering = no_ordering{})
      : game_(game), move_ordering_(move_ordering), table_(table)
  {
  }

  bool solve(value_type bound)
  {
    return _solve(bound);
  }

  move_type best_move() const
  {
    return best_move_;
  }

 private:
  constexpr bool _exceeds_opp_value(value_type bound)
  {
    if constexpr (has_opp_value<game_type>::value &&
                  has_value_bounds<game_type>::value) {
      if (game_.opp_value() >= game_type::value_bounds.second - bound) {
        return true;
      }
    }
    return false;
  }

  constexpr std::pair<bool, bool> _lookup(value_type bound, value_type value)
  {
    if constexpr (with_table<table_type>::value) {
      if (table_.is_memorable(game_)) {
        auto lookup = table_.get(game_.state());
        if (bound - value <= lookup.first) {
          return {true, true};
        }
        if (bound - value >= lookup.second) {
          return {true, false};
        }
      }
    }
    return {false, false};
  }

  constexpr std::pair<bool, bool> _abort(value_type bound, value_type value)
  {
    if (value > bound) {
      return {true, true};
    } else {
      if (_exceeds_opp_value(bound)) {
        return {true, false};
      }
    }

    if (game_.is_terminal()) {
      return {true, value > bound};
    }

    return _lookup(bound, value);
  }

  constexpr void _do_order(typename game_type::move_range& legal_actions)
  {
    if constexpr (with_ordering<move_ordering_type>::value) {
      std::sort(std::begin(legal_actions), std::end(legal_actions), move_ordering_);
    }
  }

  constexpr void __update_table(value_type bound, value_type value, bool abort)
  {
    if constexpr (with_table<table_type>::value) {
      if (table_.is_memorable(game_)) {
        if (game_.is_max() == abort) {
          table_.update_lower(game_.state(), bound - value);
        } else {
          table_.update_upper(game_.state(), bound - value);
        }
      }
    }
  }

  constexpr void _update_table_before_abort(value_type bound, value_type value)
  {
    return __update_table(bound, value, true);
  }

  constexpr void _update_table_after_no_children_left(value_type bound,
                                                      value_type value)
  {
    return __update_table(bound, value, false);
  }

  bool _solve(value_type bound, int depth = 0)
  {
    auto value = game_.value();

    auto [abort, result] = _abort(bound, value);
    if (abort) {
      return result;
    }

    auto legal_actions = game_.legal_actions();
    _do_order(legal_actions);

    for (auto a : legal_actions) {
      game_.apply_action(a);
      bool result_of_child = _solve(bound, depth + 1);
      game_.undo_action(a);

      if (result_of_child == game_.is_max()) {
        _update_table_before_abort(bound, value);

        if (depth == 0) {
          best_move_ = a;
        }
        return result_of_child;
      }
    }

    _update_table_after_no_children_left(bound, value);

    return !game_.is_max();
  }

  game_type game_;
  move_ordering_type move_ordering_;
  table_type table_;
  move_type best_move_;
};

template <Game GameT, typename LessT = no_ordering,
          typename LookupT = no_table<GameT>>
auto mws(GameT g, typename GameT::value_type test_value,
         LookupT l = no_table<GameT>{}, LessT o = no_ordering{}) {
  minimal_window_search mws(g, l, o);
  auto solution = mws.solve(test_value);
  return std::make_pair(solution, mws.best_move());
}

template <Game GameT, typename LessT = std::less<typename GameT::move_type>>
auto mws_with_memory(GameT g, typename GameT::value_type test_value, LessT o = std::less<typename GameT::move_type>{}) {
  minimal_window_search mws(g, mws_unordered_table<GameT>{}, o);
  auto solution = mws.solve(test_value);
  return std::make_pair(solution, mws.best_move());
}

template <Game GameT, typename LessT = std::less<typename GameT::move_type>>
auto mws_binary_search(GameT g, LessT o = std::less<typename GameT::move_type>{}) {
  mws_unordered_table<GameT> table{};
  minimal_window_search mws(g, table, o);

  auto value_bounds = GameT::value_bounds;
  typename GameT::value_type start = value_bounds.first,
                             end = value_bounds.second;
  auto mid = (start + end) / 2;
  bool larger = false;
  typename GameT::move_type best_move;
  while ((end - start) > 1) {
    larger = mws.solve(mid);
    if (larger) {
      start = mid;
    } else {
      end = mid;
      best_move = mws.best_move();
    }

    mid = (start + end) / 2;
    GOLV_LOG_DEBUG("start = " << start << " end = " << end);
  }
  return std::make_pair(end, best_move);  // todo: best move
}

}  // namespace golv