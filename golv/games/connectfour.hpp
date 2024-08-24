#pragma once

#include <cassert>
#include <string>
#include <vector>

namespace golv {

/**
 *   defines a connectfour game on a 7x6 board, and two players.
 */
class connectfour {
public:
  using move_type = size_t;
  using move_range = std::vector<move_type>;
  using value_type = short;

  constexpr static size_t width = 7;
  constexpr static size_t height = 6;

  enum class player_type { yellow = 1, red = -1 };

  using ext_state_type = std::vector<std::vector<player_type>>;
  using state_type = std::string;

  connectfour();

  player_type current_player() const { return current_player_; }
  move_range legal_actions() const;
  void apply_action(move_type move);
  void undo_action(move_type move);
  bool is_terminal();
  bool is_max() const { return current_player_ == player_type::yellow; }
  value_type value();
  state_type state() const;

private:
  player_type current_player_ = player_type::yellow;
  ext_state_type state_;
  value_type value_{0};
  bool is_terminal_{false};

  void switch_player();
  bool check_rows();
  bool check_cols();
  bool check_diags();

  constexpr static size_t N = 4;

  std::vector<connectfour::player_type>
  get_minor_diag(connectfour::ext_state_type const &state, size_t row,
                 size_t col);

  std::vector<connectfour::player_type>
  get_major_diag(connectfour::ext_state_type const &state, size_t row,
                 size_t col);
};

} // namespace golv
