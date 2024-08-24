#include <gtest/gtest.h>

#include <golv/games/tictactoe.hpp>

using namespace golv;

TEST(tictactoe, initial_state) 
{
    tictactoe game;
    ASSERT_EQ(game.current_player(), tictactoe::player_type::X);
    ASSERT_EQ(game.is_max(), true);
    ASSERT_EQ(game.is_terminal(), false);
    ASSERT_EQ(game.value(), 0);
    ASSERT_EQ(game.legal_actions().size(), 9);
}

TEST(tictactoe, apply_action) 
{
    tictactoe game;
    game.apply_action(0);
    ASSERT_EQ(game.current_player(), tictactoe::player_type::O);
    ASSERT_EQ(game.is_max(), false);
    ASSERT_EQ(game.is_terminal(), false);
    ASSERT_EQ(game.value(), 0);
    ASSERT_EQ(game.legal_actions().size(), 8);
    game.undo_action(0);
    ASSERT_EQ(game.current_player(), tictactoe::player_type::X);
    ASSERT_EQ(game.is_max(), true);
    ASSERT_EQ(game.is_terminal(), false);
    ASSERT_EQ(game.value(), 0);
    ASSERT_EQ(game.legal_actions().size(), 9);
}

TEST(tictactoe, apply_action_invalid)
{
    tictactoe game;
    game.apply_action(0);
    ASSERT_EQ(game.current_player(), tictactoe::player_type::O);
    ASSERT_EQ(game.is_max(), false);
    ASSERT_EQ(game.is_terminal(), false);
    ASSERT_EQ(game.value(), 0);
    ASSERT_EQ(game.legal_actions().size(), 8);
    // game.apply_action(0);
}

TEST(tictactoe, terminal_row)
{
    tictactoe game;
    game.apply_action(0);
    ASSERT_EQ(game.is_terminal(), false);
    game.apply_action(8);
    ASSERT_EQ(game.is_terminal(), false);
    game.apply_action(1);
    ASSERT_EQ(game.is_terminal(), false);
    game.apply_action(7);
    ASSERT_EQ(game.is_terminal(), false);
    game.apply_action(2);
    ASSERT_EQ(game.is_terminal(), true);
    ASSERT_EQ(game.value(), 1);
}

TEST(tictactoe, terminal_row_O)
{
    tictactoe game;
    game.apply_action(0);
    ASSERT_EQ(game.is_terminal(), false);
    game.apply_action(8);
    ASSERT_EQ(game.is_terminal(), false);
    game.apply_action(1);
    ASSERT_EQ(game.is_terminal(), false);
    game.apply_action(7);
    ASSERT_EQ(game.is_terminal(), false);
    game.apply_action(3);
    ASSERT_EQ(game.is_terminal(), false);
    game.apply_action(6);
    ASSERT_EQ(game.is_terminal(), true);
    ASSERT_EQ(game.value(), -1);
}

TEST(tictactoe, terminal_col)
{
    tictactoe game;
    game.apply_action(0);
    ASSERT_EQ(game.is_terminal(), false);
    game.apply_action(1);
    ASSERT_EQ(game.is_terminal(), false);
    game.apply_action(3);
    ASSERT_EQ(game.is_terminal(), false);
    game.apply_action(2);
    ASSERT_EQ(game.is_terminal(), false);
    game.apply_action(6);
    ASSERT_EQ(game.is_terminal(), true);
    ASSERT_EQ(game.value(), 1);
}

TEST(tictactoe, terminal_diag)
{
    tictactoe game;
    game.apply_action(0);
    ASSERT_EQ(game.is_terminal(), false);
    game.apply_action(1);
    ASSERT_EQ(game.is_terminal(), false);
    game.apply_action(4);
    ASSERT_EQ(game.is_terminal(), false);
    game.apply_action(2);
    ASSERT_EQ(game.is_terminal(), false);
    game.apply_action(8);
    ASSERT_EQ(game.is_terminal(), true);
    ASSERT_EQ(game.value(), 1);
}

TEST(tictactoe, terminal_tie)
{
    tictactoe game;
    game.apply_action(0);
    ASSERT_EQ(game.is_terminal(), false);
    game.apply_action(1);
    ASSERT_EQ(game.is_terminal(), false);
    game.apply_action(2);
    ASSERT_EQ(game.is_terminal(), false);
    game.apply_action(3);
    ASSERT_EQ(game.is_terminal(), false);
    game.apply_action(5);
    ASSERT_EQ(game.is_terminal(), false);
    game.apply_action(8);
    ASSERT_EQ(game.is_terminal(), false);
    game.apply_action(4);
    ASSERT_EQ(game.is_terminal(), false);
    game.apply_action(6);
    ASSERT_EQ(game.is_terminal(), false);
    game.apply_action(7);
    ASSERT_EQ(game.is_terminal(), true);
    ASSERT_EQ(game.value(), 0);
}