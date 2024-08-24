#pragma once

#include <string>
#include <vector>

namespace golv {

/**
 *   tictactoe represents a simple tic tac toe game with 9 fields and 2 players.
 *   This game is completely solvable for a tie.
 *   It suffices the concept Game as described in golv/state/game.hpp.
 **/
class tictactoe
{
  public:
    using move_type = short;
    using move_range = std::vector<move_type>;
    using value_type = short;

    constexpr static size_t num_fields = 9;

    enum class player_type
    {
        X,
        O
    };

    enum class field_state
    {
        empty,
        X,
        O
    };

    using internal_state_type = std::vector<field_state>;
    using state_type = std::string;

    tictactoe();

    player_type current_player() const;
    move_range legal_actions() const;
    void apply_action(move_type move);
    void undo_action(move_type move);
    bool is_terminal();
    bool is_max() const;
    value_type value();
    state_type state() const;

  private:
    player_type current_player_ = player_type::X;
    internal_state_type state_;
    value_type value_{ 0 };

    void switch_player();
};

} // namespace golv
