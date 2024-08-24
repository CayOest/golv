#include <golv/algorithm/utility.hpp>
#include <golv/games/connectfour.hpp>

namespace golv {
connectfour::move_range
connectfour::legal_actions() const
{
    move_range valid;
    for (size_t col = 0; col < connectfour::width; ++col) {
        if (state_[col].size() < height) {
            valid.push_back(col);
        }
    }
    return valid;
}

void
connectfour::switch_player()
{
    current_player_ = current_player_ == player_type::yellow ? player_type::red : player_type::yellow;
}

void
connectfour::apply_action(connectfour::move_type move)
{
    assert(state_[move].size() < height);
    state_[move].push_back(current_player_);
    switch_player();
}

void
connectfour::undo_action(connectfour::move_type move)
{
    assert(!state_[move].empty());
    state_[move].pop_back();
    is_terminal_ = false;
    switch_player();
}

bool
connectfour::check_rows()
{
    for (size_t row = 0; row < height; ++row) {
        if (state_[width / 2].size() <= row)
            break;

        auto const center = state_[width / 2][row];

        // check left half
        if (std::all_of(std::begin(state_), std::begin(state_) + width / 2, [&](auto col) {
                return col.size() > row && col[row] == center;
            })) {
            value_ = static_cast<value_type>(center);
            return true;
        }

        // check right half
        if (std::all_of(std::begin(state_) + width / 2 + 1, std::end(state_), [&](auto const& col) {
                return col.size() > row && col[row] == center;
            })) {
            value_ = static_cast<value_type>(center);
            return true;
        }
    }
    return false;
}

bool
connectfour::check_cols()
{
    // check cols
    for (auto const& col : state_) {
        if (col.size() < N)
            continue;

        auto rng = golv::longest_subsequence(std::begin(col), std::end(col));
        assert(std::distance(rng.first, rng.second) >= 0);
        if (static_cast<size_t>(std::distance(rng.first, rng.second)) >= N) {
            value_ = static_cast<value_type>(*rng.first);
            return true;
        }
    }
    return false;
}

namespace {
std::pair<size_t, size_t>
advance_equal(const connectfour::ext_state_type& state, size_t start_col, size_t start_row, bool inc_col, bool inc_row)
{
    auto const& s = state[start_col][start_row];
    auto next_row = start_row;
    auto next_col = start_col;
    do {
        inc_row ? ++next_row : --next_row;
        inc_col ? ++next_col : --next_col;
    } while (next_col < state.size() && next_row < state[next_col].size() && state[next_col][next_row] == s);
    inc_row ? --next_row : ++next_row;
    inc_col ? --next_col : ++next_col;
    return { next_col, next_row };
}
} // namespace

bool
connectfour::check_diags()
{
    auto mid = width / 2;
    for (size_t row = 0; row < state_[mid].size(); ++row) {
        auto right_rng = advance_equal(state_, mid, row, true, true);
        auto left_rng = advance_equal(state_, mid, row, false, false);
        if (right_rng.first - left_rng.first + 1 >= N) {
            value_ = static_cast<value_type>(state_[mid][row]);
            return true;
        }

        right_rng = advance_equal(state_, mid, row, true, false);
        left_rng = advance_equal(state_, mid, row, false, true);
        if (right_rng.first - left_rng.first + 1 >= N) {
            value_ = static_cast<value_type>(state_[mid][row]);
            return true;
        }
    }

    return false;
}

bool
connectfour::is_terminal()
{
    if (is_terminal_)
        return true;

    if (check_rows() || check_cols() || check_diags()) {
        is_terminal_ = true;
    } else {
        value_ = 0;
        // check if all cols are full
        is_terminal_ =
          std::all_of(std::begin(state_), std::end(state_), [](auto col) { return col.size() == connectfour::height; });
    }
    return is_terminal_;
}

connectfour::value_type
connectfour::value()
{
    if (is_terminal())
        return value_;
    else
        return 0;
}

connectfour::connectfour()
  : state_{ width, std::vector<player_type>{} }
{
}

connectfour::state_type
connectfour::state() const
{
    // convert ext_state_type to state_type
    state_type state;
    for (auto const& col : state_) {
        for (auto const& entry : col) {
            state += entry == player_type::yellow ? "y" : "r";
        }
        for (auto i = col.size(); i < height; ++i) {
            state += " ";
        }
    }
    return state;
}

} // namespace golv