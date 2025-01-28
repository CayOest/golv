#include <golv/algorithm/move_ordering.hpp>
#include <golv/algorithm/mws_unordered_table.hpp>
#include <golv/traits/game.hpp>

namespace golv {
template <Game GameT, typename MoveOrderingT = no_ordering, TranspositionTable<GameT> TableT = no_table<GameT>>
class minimal_window_search {
 public:
  using game_type = GameT;
  using value_type = typename game_type::value_type;
  using move_type = typename game_type::move_type;
  using move_ordering_type = MoveOrderingT;
  using table_type = TableT;

  constexpr static value_type min_value = std::numeric_limits<value_type>::lowest() / 2;
  constexpr static value_type max_value = std::numeric_limits<value_type>::max() / 2;

  minimal_window_search(GameT game, MoveOrderingT move_ordering = no_ordering{}, TableT table = no_table<game_type>{})
      : game_(game), move_ordering_(move_ordering), table_(table) {}

  bool solve(value_type bound) { return _solve(bound); }

  bool _solve(value_type bound, int depth = 0) {
    // if (value > bound) return true;
    auto value = game_.value();
    if (game_.is_terminal()) {
      return value > bound;
    }
    // auto pts = game_.counts();
    // if (pts.first > bound) return true;
    // else if (pts.second >= totalEyes_ - bound) return false;

    // bool isMax = game_.currentIsSingle();
    // bool isHash = game_.currentIsLead();

    // uint64_t hash = 0;
    if constexpr (with_table<table_type>::value) {
      if (table_.is_memorable(game_)) {
        auto lookup = table_.get(game_.state());
        if (bound - value <= lookup.first) {
          return true;
        }
        if (bound - value >= lookup.second) {
          return false;
        }
      }
    }
    // if (isHash) {
    // 	hash = game_.getState();
    // 	auto val = tt_.readLower(hash);
    // 	if (bound - pts.first <= val) {
    // 		return true;
    // 	}
    // 	val = tt_.readUpper(hash);
    // 	if (bound - pts.first >= val) {
    // 		return false;
    // 	}
    // }
    auto legal_actions = game_.legal_actions();

    if constexpr (with_ordering<move_ordering_type>::value) {
      std::sort(std::begin(legal_actions), std::end(legal_actions), move_ordering_);
    }

    for (auto a : legal_actions) {
      game_.apply_action(a);
      // value_type move_value = game_.value();
      // accumulated_value += move_value;
      bool son = _solve(bound, depth + 1);
      game_.undo_action(a);

      // game_.advance(*it);
      // son = _solve(bound, depth + 1);
      // game_.regress();

      if (son == game_.is_max()) {
        if constexpr (with_table<table_type>::value) {
          if (table_.is_memorable(game_)) {
            if (game_.is_max()) {
              table_.update_lower(game_.state(), bound - value);
            } else {
              table_.update_upper(game_.state(), bound - value);
            }
          }
        }
        if (depth == 0) {
          best_move_ = a;
        }
        return son;
      }

      // if (son == isMax) {
      // 	if (isHash) {
      // 		if (isMax)
      // 			tt_.updateLower(hash, bound - pts.first);
      // 		else
      // 			tt_.updateUpper(hash, bound - pts.first);
      // 	}
      // 	if (depth == 0)
      // 		solution_.card = { *it };
      // 	return son;
      // }
    }

    if constexpr (with_table<table_type>::value) {
      if (table_.is_memorable(game_)) {
        if (game_.is_max()) {
          table_.update_upper(game_.state(), bound - value);
        } else {
          table_.update_lower(game_.state(), bound - value);
        }
      }
    }

    // if (isHash && hash != 0) {
    // 	if (!isMax) {
    // 		tt_.updateLower(hash, bound - pts.first);
    // 	}
    // 	else {
    // 		tt_.updateUpper(hash, bound - pts.first);
    // 	}
    // }

    return !game_.is_max();
  }

  move_type best_move() const { return best_move_; }

  game_type game_;
  move_ordering_type move_ordering_;
  table_type table_;
  move_type best_move_;
};

template <Game GameT, typename LessT = no_ordering, typename LookupT = no_table<GameT>>
auto mws(GameT g, typename GameT::value_type test_value, LessT o = no_ordering{}, LookupT l = no_table<GameT>{}) {
  minimal_window_search mws(g, o, l);
  auto solution = mws.solve(test_value);
  return std::make_pair(solution, mws.best_move());
}

template <Game GameT, typename LessT = std::less<typename GameT::move_type>>
auto mws_with_memory(GameT g, typename GameT::value_type test_value, LessT o = std::less<typename GameT::move_type>{}) {
  minimal_window_search mws(g, o, mws_unordered_table<GameT>{});
  auto solution = mws.solve(test_value);
  return std::make_pair(solution, mws.best_move());
}

}  // namespace golv