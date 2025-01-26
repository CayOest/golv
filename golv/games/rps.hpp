#pragma once

#include <string>
#include <vector>

namespace golv {

/**
 *   rock_paper_scissors represents a simple rock/paper/scissors game with 2
 *players. This game has an optimal strategy (1/3, 1/3, 1/3). It suffices the
 *concept Game as described in golv/traits/game.hpp.
 **/
class rock_paper_scissors {
  public:
    using value_type = short;
    using player_type = bool;
    using state_type = std::string;

    using move_type = char;
    using move_range = std::string;

    using strategy_type = std::vector<double>;
    using information_set_type = move_range;

    state_type state_;

    int max_player_ = 0;
    int curr_player_ = 0;

    move_range legal_actions() const { return "rps"; }

    void set_max(int i) { max_player_ = i; }

    player_type current_player() const { return state_.size() == 0 ? 0 : 1; }

    constexpr bool is_chance_node() const { return false; }

    // empty
    constexpr void handle_chance_node() const {}

    void apply_action(move_type move) {
      static const move_range _legal_actions = legal_actions();

      auto it = std::find(_legal_actions.begin(), _legal_actions.end(), move);
      if (it == _legal_actions.end())
        throw std::domain_error("Wrong move: " + move);
      state_ += move;
      curr_player_ = (curr_player_ + 1) % 2;
    }

    void undo_action(move_type move) {
      if (!state_.empty()) {
        if (state_.back() == move) state_.pop_back();
      }
      curr_player_ = (curr_player_ + 1) % 2;
    }

    bool is_terminal() const { return state_.size() == 2; }

    bool is_max() const { return max_player_ == curr_player_; }

    state_type state() const { return state_; };

    value_type value() const {
      auto val = _value();
      return is_max() ? val : -val;
    }

    value_type _value() const {
      if (state_.size() == 2) {
        if (state_[0] == state_[1]) return 0;
        if (state_[0] == 'r') return state_[1] == 'p' ? -1 : +1;
        if (state_[0] == 'r') return state_[1] == 's' ? +1 : -1;
        if (state_[0] == 'p') return state_[1] == 'r' ? +1 : -1;
        if (state_[0] == 'p') return state_[1] == 's' ? -1 : +1;
        if (state_[0] == 's') return state_[1] == 'r' ? -1 : +1;
        if (state_[0] == 's') return state_[1] == 'p' ? +1 : -1;
      }
      return 0;
    }
};

}  // namespace golv