#include <golv/games/tictactoe.hpp>

#include <cassert>

namespace golv {

tictactoe::move_range
tictactoe::legal_actions() const
{
    move_range valid;
    for (int i = 0; i < 9; ++i) {
        if (state_[i] == field_state::empty) {
            valid.push_back(i);
        }
    }
    return valid;
}

void
tictactoe::switch_player()
{
    current_player_ = current_player_ == player_type::X ? player_type::O : player_type::X;
}

void
tictactoe::apply_action(tictactoe::move_type move)
{
    assert(state_[move] == field_state::empty);
    state_[move] = current_player_ == player_type::X ? field_state::X : field_state::O;
    switch_player();
}

void
tictactoe::undo_action(tictactoe::move_type move)
{
    assert(state_[move] != field_state::empty);
    state_[move] = field_state::empty;
    switch_player();
}

bool
tictactoe::is_terminal()
{
    // check rows
    for (int row = 0; row < 9; row += 3) {
        if (state_[row] != field_state::empty && state_[row] == state_[row + 1] && state_[row] == state_[row + 2]) {
            value_ = state_[row] == field_state::X ? 1 : -1;
            return true;
        }
    }
    // check cols
    for (int col = 0; col < 3; ++col) {
        if (state_[col] != field_state::empty && state_[col] == state_[col + 3] && state_[col] == state_[col + 6]) {
            value_ = state_[col] == field_state::X ? 1 : -1;
            return true;
        }
    }
    // check diags
    if (state_[4] != field_state::empty &&
        (state_[0] == state_[4] && state_[0] == state_[8] || state_[2] == state_[4] && state_[2] == state_[6])) {
        value_ = state_[4] == field_state::X ? 1 : -1;
        return true;
    }
    value_ = 0;
    return std::all_of(state_.begin(), state_.end(), [](auto s) { return s != field_state::empty; });
}

tictactoe::value_type
tictactoe::value()
{
    if (is_terminal())
        return value_;
    else
        return 0;
}

tictactoe::state_type
tictactoe::state() const
{
    state_type state;
    for (auto const& field : state_) {
        state += field == field_state::empty ? " " : field == field_state::X ? "X" : "O";
    }
    return state;
}

tictactoe::tictactoe()
  : state_{ num_fields, field_state::empty }
{
}

bool
tictactoe::is_max() const
{
    return current_player_ == player_type::X;
}

tictactoe::player_type
tictactoe::current_player() const
{
    return current_player_;
}

} // namespace golv