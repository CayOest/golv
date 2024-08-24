#include <gtest/gtest.h>

#include <golv/games/connectfour.hpp>

using namespace golv;

TEST(connectfour, initial_state)
{
    connectfour game;
    ASSERT_EQ(game.current_player(), connectfour::player_type::yellow);
    ASSERT_EQ(game.is_max(), true);
    ASSERT_EQ(game.is_terminal(), false);
    ASSERT_EQ(game.value(), 0);
    ASSERT_EQ(game.legal_actions().size(), connectfour::width);
}

TEST(connectfour, apply_action)
{
    connectfour game;
    game.apply_action(0);
    ASSERT_EQ(game.current_player(), connectfour::player_type::red);
    ASSERT_EQ(game.is_max(), false);
    ASSERT_EQ(game.is_terminal(), false);
    ASSERT_EQ(game.value(), 0);
    ASSERT_EQ(game.legal_actions().size(), connectfour::width);
    game.undo_action(0);
    ASSERT_EQ(game.current_player(), connectfour::player_type::yellow);
    ASSERT_EQ(game.is_max(), true);
    ASSERT_EQ(game.is_terminal(), false);
    ASSERT_EQ(game.value(), 0);
    ASSERT_EQ(game.legal_actions().size(), connectfour::width);
}

// TODO!
TEST(connectfour, apply_action_invalid)
{
    connectfour game;
    game.apply_action(0);
    ASSERT_EQ(game.current_player(), connectfour::player_type::red);
    ASSERT_EQ(game.is_max(), false);
    ASSERT_EQ(game.is_terminal(), false);
    ASSERT_EQ(game.value(), 0);
    ASSERT_EQ(game.legal_actions().size(), connectfour::width);
    // game.apply_action(0);
}

TEST(connectfour, terminal_col)
{
    connectfour game;
    game.apply_action(0);
    ASSERT_EQ(game.is_terminal(), false);
    game.apply_action(1);
    ASSERT_EQ(game.is_terminal(), false);
    game.apply_action(0);
    ASSERT_EQ(game.is_terminal(), false);
    game.apply_action(1);
    ASSERT_EQ(game.is_terminal(), false);
    game.apply_action(0);
    ASSERT_EQ(game.is_terminal(), false);
    game.apply_action(1);
    ASSERT_EQ(game.is_terminal(), false);
    game.apply_action(0);
    ASSERT_EQ(game.is_terminal(), true);
    ASSERT_EQ(game.value(), 1);
    game.undo_action(0);
    ASSERT_EQ(game.is_terminal(), false);
    game.apply_action(2);
    ASSERT_EQ(game.is_terminal(), false);
    game.apply_action(1);
    ASSERT_EQ(game.is_terminal(), true);
}

TEST(connectfour, terminal_row)
{
    connectfour game;
    game.apply_action(0);
    ASSERT_EQ(game.is_terminal(), false);
    game.apply_action(6);
    ASSERT_EQ(game.is_terminal(), false);
    game.apply_action(1);
    ASSERT_EQ(game.is_terminal(), false);
    game.apply_action(6);
    ASSERT_EQ(game.is_terminal(), false);
    game.apply_action(2);
    ASSERT_EQ(game.is_terminal(), false);
    game.apply_action(6);
    ASSERT_EQ(game.is_terminal(), false);
    game.apply_action(3);
    ASSERT_EQ(game.is_terminal(), true);
    ASSERT_EQ(game.value(), 1);
    game.undo_action(3);
    ASSERT_EQ(game.is_terminal(), false);
    game.apply_action(4);
    ASSERT_EQ(game.is_terminal(), false);
    game.apply_action(6);
    ASSERT_EQ(game.is_terminal(), true);
}

TEST(connectfour, terminal_diag)
{
    connectfour game;
    game.apply_action(0);
    ASSERT_EQ(game.is_terminal(), false);
    game.apply_action(1);
    ASSERT_EQ(game.is_terminal(), false);
    game.apply_action(1);
    ASSERT_EQ(game.is_terminal(), false);
    game.apply_action(2);
    ASSERT_EQ(game.is_terminal(), false);
    game.apply_action(3);
    ASSERT_EQ(game.is_terminal(), false);
    game.apply_action(2);
    ASSERT_EQ(game.is_terminal(), false);
    game.apply_action(2);
    ASSERT_EQ(game.is_terminal(), false);
    game.apply_action(3);
    ASSERT_EQ(game.is_terminal(), false);
    game.apply_action(3);
    ASSERT_EQ(game.is_terminal(), false);
    game.apply_action(4);
    ASSERT_EQ(game.is_terminal(), false);
    game.apply_action(3);
    ASSERT_EQ(game.is_terminal(), true);
    ASSERT_EQ(game.value(), 1);
}

TEST(connectfour, connectfour_terminal)
{
    golv::connectfour game;
    game.apply_action(0);
    game.apply_action(0);
    game.apply_action(0);
    game.apply_action(0);
    game.apply_action(0);
    game.apply_action(0);

    game.apply_action(1);
    game.apply_action(1);
    game.apply_action(1);
    game.apply_action(1);
    game.apply_action(1);
    game.apply_action(1);

    game.apply_action(3);
    game.apply_action(2);
    game.apply_action(3);
    game.apply_action(3);
    game.apply_action(3);
    game.apply_action(3);

    game.apply_action(3);
    game.apply_action(5);
    game.apply_action(2);
    game.apply_action(5);

    ASSERT_FALSE(game.is_terminal());
}

TEST(connectfour, legal_actions)
{
    connectfour game;
    ASSERT_EQ(game.legal_actions().size(), golv::connectfour::width);
    game.apply_action(0);
    game.apply_action(0);
    game.apply_action(0);
    game.apply_action(0);
    game.apply_action(0);
    ASSERT_EQ(game.legal_actions().size(), golv::connectfour::width);
    game.apply_action(0);
    ASSERT_EQ(game.legal_actions().size(), golv::connectfour::width - 1);
}

// TODO!
TEST(connectfour, tie) { }