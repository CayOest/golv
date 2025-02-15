#include <gtest/gtest.h>

#include <golv/games/skat.hpp>
#include <golv/util/logging.hpp>
#include <golv/util/test_utils.hpp>
#include <iostream>

#include "../util/test_games.hpp"

using namespace golv;

TEST(skat, legal) {
  skat game = default_skat_game_10(0, 1);
  GOLV_LOG_INFO("" << game);
  ASSERT_EQ(game.current_player(), 0);
  auto moves = game.legal_actions();
  ASSERT_EQ(moves.size(), 10);
}

TEST(skat, apply_action) {
  skat game = default_skat_game_10();
  auto moves = game.legal_actions();
  EXPECT_NO_THROW(game.apply_action(moves.front()));
}

TEST(skat, apply_action_invalid) {
  skat game = default_skat_game_10();
  auto moves = game.legal_actions();
  EXPECT_NO_THROW(game.apply_action(moves.front()));
  EXPECT_THROW(game.apply_action(moves.front()), golv::exception);
}

TEST(skat, undo_action) {
  skat game = default_skat_game_10();
  auto moves = game.legal_actions();
  EXPECT_NO_THROW(game.apply_action(moves.front()));
  EXPECT_NO_THROW(game.undo_action(moves.front()));
}

TEST(skat, undo_action_invalid) {
  skat game = default_skat_game_10();
  auto moves = game.legal_actions();
  EXPECT_NO_THROW(game.apply_action(moves.front()));
  EXPECT_THROW(game.undo_action(moves.back()), golv::exception);
}

TEST(skat, legal_after_action) {
  skat game = default_skat_game_10(0, 1);
  ASSERT_EQ(game.current_player(), 0);
  auto moves = game.legal_actions();
  ASSERT_EQ(moves.size(), 10);
  game.apply_action(moves.back());
  moves = game.legal_actions();
  ASSERT_EQ(moves.size(), 2);
}

TEST(skat, new_trick) {
  skat game = default_skat_game_10();
  auto moves = game.legal_actions();
  EXPECT_NO_THROW(game.apply_action(moves.front()));
  moves = game.legal_actions();
  EXPECT_NO_THROW(game.apply_action(moves.front()));
  moves = game.legal_actions();
  EXPECT_NO_THROW(game.apply_action(moves.front()));
  auto tricks = game.tricks();
  ASSERT_EQ(tricks.size(), 2);
  // new trick
  moves = game.legal_actions();
  ASSERT_EQ(moves.size(), 9);
}

TEST(skat, undo_after_trick) {
  skat game = default_skat_game_10();
  auto moves = game.legal_actions();
  EXPECT_NO_THROW(game.apply_action(moves.front()));
  moves = game.legal_actions();
  EXPECT_NO_THROW(game.apply_action(moves.front()));
  moves = game.legal_actions();
  EXPECT_NO_THROW(game.apply_action(moves.front()));
  auto tricks = game.tricks();
  ASSERT_EQ(tricks.size(), 2);
  // new trick
  EXPECT_NO_THROW(game.undo_action(moves.front()));
  tricks = game.tricks();
  ASSERT_EQ(tricks.size(), 1);
  ASSERT_EQ(tricks.front().cards_.size(), 2);
}

TEST(skat, trick_winner) {
  skat game = default_skat_game_10();
  auto moves = game.legal_actions();
  EXPECT_NO_THROW(game.apply_action(moves.front()));
  moves = game.legal_actions();
  EXPECT_NO_THROW(game.apply_action(moves.front()));
  moves = game.legal_actions();
  EXPECT_NO_THROW(game.apply_action(moves.front()));
  auto tricks = game.tricks();
  ASSERT_EQ(tricks.size(), 2);
  // new trick
  auto winner = tricks.back().leader_;
  ASSERT_EQ(winner, 2);
  ASSERT_EQ(game.value(), 10);
  moves = game.legal_actions();
  EXPECT_NO_THROW(game.apply_action(moves.front()));
  moves = game.legal_actions();
  EXPECT_NO_THROW(game.apply_action(moves.front()));
  moves = game.legal_actions();
  EXPECT_NO_THROW(game.apply_action(moves.front()));
  tricks = game.tricks();
  ASSERT_EQ(tricks.size(), 3);
  winner = tricks.back().leader_;
  ASSERT_EQ(winner, 0);
  ASSERT_EQ(game.value(), 14);
  game.undo_action(moves.front());
  ASSERT_EQ(game.value(), 10);
}

TEST(skat, trick_1) {
  golv::skat game = default_skat_game_10(1, 0);
  GOLV_LOG_DEBUG("game = " << game);
  game.apply_action("Ad");
  game.apply_action("Qd");
  auto moves = game.legal_actions();
  game.apply_action("9d");
  auto winner = game.tricks().back().leader_;
  ASSERT_EQ(winner, 0);
  auto value = game.value();
  ASSERT_EQ(value, 24);
  ASSERT_TRUE(game.is_max());
  GOLV_LOG_DEBUG("game = " << game.state());
  game.undo_action("9d");
  GOLV_LOG_DEBUG("game = " << game.state());
  value = game.value();
  ASSERT_EQ(value, 10);
}

TEST(skat, legal_1) {
  golv::skat game = default_skat_game_10();
  auto moves = game.legal_actions();
  GOLV_LOG_DEBUG("game = " << game);
  game.apply_action("Ac");
  auto legal = game.legal_actions();
  ASSERT_EQ(legal.size(), 2);
  ASSERT_EQ(legal.front(), "8c");
  ASSERT_EQ(legal.back(), "Tc");
  game.apply_action("8c");
  legal = game.legal_actions();
  ASSERT_EQ(legal.size(), 2);
  ASSERT_EQ(legal.front(), "Qc");
  ASSERT_EQ(legal.back(), "Kc");
}

TEST(skat, legal_jack) {
  golv::skat game = default_skat_game_10(1);
  auto moves = game.legal_actions();
  GOLV_LOG_DEBUG("game = " << game);
  game.apply_action("Jh");
  auto legal = game.legal_actions();
  ASSERT_EQ(legal.size(), 2);
  ASSERT_EQ(legal.front(), "Jd");
  ASSERT_EQ(legal.back(), "Js");
  game.apply_action("Jd");
  legal = game.legal_actions();
  ASSERT_EQ(legal.size(), 10);
}