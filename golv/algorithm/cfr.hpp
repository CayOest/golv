#pragma once

#include <golv/traits/game.hpp>

namespace golv {

template <Game GameT>
class cfr {
  public:
    using game_type = GameT;
    using value_type = typename game_type::value_type;
    using strategy_type = typename game_type::strategy_type;
    using information_set_type = typename game_type::information_set_type;
    using map_type = std::map<information_set_type, strategy_type>;

    cfr(GameT game) : game_(game) {}

    auto solve() -> void {
      if (game_.is_terminal()) return game_.value();
      if (game_.is_chance_node()) {
        // do stuff
        return 0;
      }
      for (auto const& a : game_.legal_actions()) {
      }
    }

  private:
    game_type game_;

    map_type map_;
};

}  // namespace golv