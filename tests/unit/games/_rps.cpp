#include <gtest/gtest.h>

#include <golv/games/rps.hpp>

using namespace golv;

TEST(rps, initial_state) {
  rock_paper_scissors game;
  ASSERT_EQ(game.current_player(), 0);
  ASSERT_EQ(game.is_max(), true);
  ASSERT_EQ(game.is_terminal(), false);
  ASSERT_EQ(game.value(), 0);
  ASSERT_EQ(game.legal_actions().size(), 3);
}

TEST(rps, apply_action) {
  rock_paper_scissors game;
  game.apply_action('r');
  ASSERT_EQ(game.current_player(), 1);
  ASSERT_EQ(game.is_max(), false);
  ASSERT_EQ(game.is_terminal(), false);
  ASSERT_EQ(game.value(), 0);
  game.apply_action('p');
  ASSERT_EQ(game.is_terminal(), true);
  ASSERT_EQ(game.value(), -1);
  game.undo_action('p');
  ASSERT_EQ(game.is_terminal(), false);
  ASSERT_EQ(game.value(), 0);
  game.apply_action('s');
  ASSERT_EQ(game.is_terminal(), true);
  ASSERT_EQ(game.value(), 1);
}

TEST(rps, apply_action_invalid) {
  rock_paper_scissors game;
  ASSERT_THROW(game.apply_action('x'), std::domain_error);
}