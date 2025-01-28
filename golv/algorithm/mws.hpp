#include <golv/algorithm/move_ordering.hpp>
#include <golv/algorithm/unordered_table.hpp>
#include <golv/traits/game.hpp>

namespace golv {
template <Game GameT, typename MoveOrderingT = no_ordering, TranspositionTable<GameT> TableT = no_table<GameT>>
class mws {
 public:
  using game_type = GameT;
  using value_type = typename game_type::value_type;
  using move_type = typename game_type::move_type;
  using move_ordering_type = MoveOrderingT;
  using table_type = TableT;

  constexpr static value_type min_value = std::numeric_limits<value_type>::lowest() / 2;
  constexpr static value_type max_value = std::numeric_limits<value_type>::max() / 2;

  mws(GameT game, MoveOrderingT move_ordering = no_ordering{}, TableT table = no_table<game_type>{})
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
        if (lookup.first == lookup_value_type::lower_bound) {
          if (bound - value <= lookup.second) {
            return true;
          }
        }
        if (lookup.first == lookup_value_type::upper_bound) {
          if (bound - value >= lookup.second) {
            return false;
          }
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
    // auto childrenRange = game_.validCards();
    // auto children = std::vector<Card>(childrenRange.first, childrenRange.second);

    bool son = false;

    for (auto a : legal_actions) {
      game_.apply_action(a);
      // value_type move_value = game_.value();
      // accumulated_value += move_value;
      son = _solve(bound, depth + 1);
      game_.undo_action(a);

      // game_.advance(*it);
      // son = _solve(bound, depth + 1);
      // game_.regress();

      if (son == game_.is_max()) {
        if (game_.is_max()) {
          _save_value(lookup_value_type::lower_bound, bound - value);
        } else {
          _save_value(lookup_value_type::upper_bound, bound - value);
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
    if (game_.is_max()) {
      _save_value(lookup_value_type::upper_bound, bound - value);
    } else {
      _save_value(lookup_value_type::lower_bound, bound - value);
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

  void _save_value(lookup_value_type lookup_value, value_type value) {
    if constexpr (with_table<table_type>::value) {
      if (table_.is_memorable(game_)) {
        table_.set(game_.state(), lookup_value, value);
      }
    }
  }

  game_type game_;
  move_ordering_type move_ordering_;
  table_type table_;
  move_type best_move_;
};

}  // namespace golv