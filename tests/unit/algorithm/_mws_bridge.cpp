#include <gtest/gtest.h>

#include <algorithm>
#include <functional>
#include <golv/algorithm/mws.hpp>
#include <golv/util/logging.hpp>
#include <golv/util/test_utils.hpp>

#include "../util/test_games.hpp"

using namespace golv;

class mws_bridge : public ::testing::Test {
 protected:
  void SetUp() override { golv::set_log_level(golv::log_level::debug); }
};

TEST_F(mws_bridge, bridge_5cps) {
  auto game = default_game_5();
  GOLV_LOG_DEBUG("game = " << game.state());

  auto expected = 4;
  auto [lower, best_move] = mws(game, expected - 1);
  ASSERT_TRUE(lower);
  auto [upper, not_best_move] = mws(game, expected);
  ASSERT_FALSE(upper);
}

TEST_F(mws_bridge, bridge_5cps_with_mem) {
  auto game = default_game_5();
  GOLV_LOG_DEBUG("game = " << game.state());

  auto expected = 4;
  auto [lower, best_move] = mws_with_memory(game, expected - 1);
  ASSERT_TRUE(lower);
  auto [upper, not_best_move] = mws_with_memory(game, expected);
  ASSERT_FALSE(upper);
}

TEST_F(mws_bridge, bridge_5cps_rot1) {
  auto game = default_game_5(1);
  GOLV_LOG_DEBUG("game = " << game.state());

  auto expected = 5;
  auto [lower, best_move] = mws(game, expected - 1);
  ASSERT_TRUE(lower);
  auto [upper, not_best_move] = mws(game, expected);
  ASSERT_FALSE(upper);
}

TEST_F(mws_bridge, bridge_5cps_rot1_with_mem) {
  auto game = default_game_5(1);
  GOLV_LOG_DEBUG("game = " << game.state());

  auto expected = 5;
  auto [lower, best_move] = mws_with_memory(game, expected - 1);
  ASSERT_TRUE(lower);
  auto [upper, not_best_move] = mws_with_memory(game, expected);
  ASSERT_FALSE(upper);
}

TEST_F(mws_bridge, bridge_5cps_solo1) {
  auto game = default_game_5();
  game.set_soloist(1);
  GOLV_LOG_DEBUG("game = " << game.state());

  auto expected = 1;
  auto [lower, best_move] = mws(game, expected - 1);
  ASSERT_TRUE(lower);
  auto [upper, not_best_move] = mws(game, expected);
  ASSERT_FALSE(upper);
}

TEST_F(mws_bridge, bridge_5cps_solo1_with_mem) {
  auto game = default_game_5();
  game.set_soloist(1);
  GOLV_LOG_DEBUG("game = " << game.state());

  auto expected = 1;
  auto [lower, best_move] = mws_with_memory(game, expected - 1);
  ASSERT_TRUE(lower);
  auto [upper, not_best_move] = mws_with_memory(game, expected);
  ASSERT_FALSE(upper);
}

TEST_F(mws_bridge, bridge_7cps_with_memory) {
  auto game = create_random_game(7);
  GOLV_LOG_DEBUG("game = " << game.state());

  auto expected = 2;
  auto [lower, best_move] = mws_with_memory(game, expected - 1);
  ASSERT_TRUE(lower);
  auto [upper, not_best_move] = mws_with_memory(game, expected);
  ASSERT_FALSE(upper);
}

TEST_F(mws_bridge, skat_7cps_with_memory) {
  auto game = create_random_skat_game(7, 1);
  GOLV_LOG_DEBUG("game = " << game);

  auto expected = 36;
  auto [lower, best_move] = mws_with_memory(game, expected - 1);
  ASSERT_TRUE(lower);
  auto [upper, not_best_move] = mws_with_memory(game, expected);
  ASSERT_FALSE(upper);
}

TEST_F(mws_bridge, skat_8cps_with_mem) {
  auto game = create_random_skat_game(8, 2);
  GOLV_LOG_DEBUG("game = " << game);

  auto [value, best_move] = mws_binary_search(game);

  ASSERT_EQ(value, 32);
}

TEST_F(mws_bridge, skat_10cps_with_mem) {
  auto game = default_skat_game_10();
  GOLV_LOG_DEBUG("game = " << game);

  auto [value, best_move] = mws_binary_search(game);
  GOLV_LOG_DEBUG("best_move = " << best_move);

  ASSERT_EQ(value, 24);
  ASSERT_EQ(best_move, "Ac");
}

TEST_F(mws_bridge, skat_10cps_with_mem_rot1) {
  auto game = default_skat_game_10(1);
  GOLV_LOG_DEBUG("game = " << game);

  auto [value, bm] = mws_binary_search(game);
  GOLV_LOG_DEBUG("value = " << value << ", bm = " << bm);
  ASSERT_EQ(value, 36);
}

TEST_F(mws_bridge, skat_10cps_with_mem_rot2) {
  auto game = default_skat_game_10(2);
  GOLV_LOG_DEBUG("game = " << game);

  auto [value, bm] = mws_binary_search(game);
  GOLV_LOG_DEBUG("value = " << value << ", bm = " << bm);
  ASSERT_EQ(value, 27);
  ASSERT_EQ(bm, "As");
}