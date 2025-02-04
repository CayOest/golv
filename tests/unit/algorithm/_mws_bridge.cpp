#include <gtest/gtest.h>

#include <algorithm>
#include <functional>
#include <golv/algorithms/mws.hpp>
#include <golv/util/logging.hpp>
#include <golv/util/test_utils.hpp>

#include "../util/test_games.hpp"

using namespace golv;

class mws_bridge : public ::testing::Test {
 protected:
  void SetUp() override
  {
    golv::set_log_level(golv::log_level::debug);
  }
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

TEST_F(mws_bridge, bridge_5cards_with_mem) {
  auto game = default_game_5();
  GOLV_LOG_DEBUG("game = " << game.state());

  auto expected = 4;
  auto [lower, best_move] = mws_with_memory(game, expected - 1);
  ASSERT_TRUE(lower);
  auto [upper, not_best_move] = mws_with_memory(game, expected);
  ASSERT_FALSE(upper);
}

TEST_F(mws_bridge, bridge_5cards_rot1) {
  auto game = default_game_5(1);
  GOLV_LOG_DEBUG("game = " << game.state());

  auto expected = 5;
  auto [lower, best_move] = mws(game, expected - 1);
  ASSERT_TRUE(lower);
  auto [upper, not_best_move] = mws(game, expected);
  ASSERT_FALSE(upper);
}

TEST_F(mws_bridge, bridge_5cards_rot1_with_mem) {
  auto game = default_game_5(1);
  GOLV_LOG_DEBUG("game = " << game.state());

  auto expected = 5;
  auto [lower, best_move] = mws_with_memory(game, expected - 1);
  ASSERT_TRUE(lower);
  auto [upper, not_best_move] = mws_with_memory(game, expected);
  ASSERT_FALSE(upper);
}

TEST_F(mws_bridge, bridge_5cards_solo1) {
  auto game = default_game_5();
  game.set_soloist(1);
  GOLV_LOG_DEBUG("game = " << game.state());

  auto expected = 1;
  auto [lower, best_move] = mws(game, expected - 1);
  ASSERT_TRUE(lower);
  auto [upper, not_best_move] = mws(game, expected);
  ASSERT_FALSE(upper);
}

TEST_F(mws_bridge, bridge_5cards_solo1_with_mem) {
  auto game = default_game_5();
  game.set_soloist(1);
  GOLV_LOG_DEBUG("game = " << game.state());

  auto expected = 1;
  auto [lower, best_move] = mws_with_memory(game, expected - 1);
  ASSERT_TRUE(lower);
  auto [upper, not_best_move] = mws_with_memory(game, expected);
  ASSERT_FALSE(upper);
}

#if 0
TEST_F(mws_bridge, bridge_7cards_with_memory) {
  auto game = create_random_game(7);
  GOLV_LOG_DEBUG("game = " << game.state());

  auto expected = 2;
  auto [lower, best_move] = mws_with_memory(game, expected - 1);
  ASSERT_TRUE(lower);
  auto [upper, not_best_move] = mws_with_memory(game, expected);
  ASSERT_FALSE(upper);
}

#ifdef NDEBUG

TEST_F(mws_bridge, bridge_9cards_binary) {
  auto game = create_random_game(9);
  GOLV_LOG_DEBUG("game = " << game.state());

  auto [value, best_move] = mws_binary_search(game);
  ASSERT_EQ(value, 9);
}

TEST_F(mws_bridge, bridge_13cards_binary) {
  auto game = create_random_game(13);
  GOLV_LOG_DEBUG("game = " << game.state());

  auto [value, best_move] = mws_binary_search(game);
  ASSERT_EQ(value, 9);
}
#endif
#endif

TEST_F(mws_bridge, skat_7cards_with_memory) {
  auto game = default_skat_game_7(1);
  GOLV_LOG_DEBUG("game = " << game);

  auto expected = 28;
  auto [lower, best_move] = mws_with_memory(game, expected - 1);
  ASSERT_TRUE(lower);
  auto [upper, not_best_move] = mws_with_memory(game, expected);
  ASSERT_FALSE(upper);
}

struct order {
  skat_card_order skat_order{suit::clubs};
  bool operator()(const card& left, const card& right) {          //
    return !operator==(left, right) && !skat_order(left, right);  //
  }
};

TEST_F(mws_bridge, skat_10cards_binary)
{
  auto game = default_skat_game_10();
  GOLV_LOG_DEBUG("game = " << game);

  auto actions = game.legal_actions();
  std::sort(actions.begin(), actions.end(), order{});
  GOLV_LOG_DEBUG("actions = " << actions);

  auto [value, best_move] =  // mws_binary_search(game);
      mws_binary_search<golv::skat, order>(game, order{});
  GOLV_LOG_DEBUG("best_move = " << best_move);

  ASSERT_EQ(value, 24);
  ASSERT_EQ(best_move, "Ac");
}

TEST_F(mws_bridge, skat_10cards_binary_rot1)
{
  auto game = default_skat_game_10(1);
  GOLV_LOG_DEBUG("game = " << game);

  auto actions = game.legal_actions();
  std::sort(actions.begin(), actions.end(), order{});
  GOLV_LOG_DEBUG("actions = " << actions);

  auto [value, bm] =  // mws_binary_search(game);
      mws_binary_search<golv::skat, order>(game, order{});
  GOLV_LOG_DEBUG("value = " << value << ", bm = " << bm);
  ASSERT_EQ(value, 36);
}

TEST_F(mws_bridge, skat_10cards_binary_rot2)
{
  auto game = default_skat_game_10(2);
  GOLV_LOG_DEBUG("game = " << game);

  auto actions = game.legal_actions();
  std::sort(actions.begin(), actions.end(), order{});
  GOLV_LOG_DEBUG("actions = " << actions);

  auto [value, bm] =  // mws_binary_search(game);
      mws_binary_search<golv::skat, order>(game, order{});
  GOLV_LOG_DEBUG("value = " << value << ", bm = " << bm);
  ASSERT_EQ(value, 27);
  ASSERT_EQ(bm, "As");
}

TEST_F(mws_bridge, skat_10cards_binary_with_pushing)
{
  auto game = default_skat_game_10(0, 0, false);
  GOLV_LOG_DEBUG("game = " << game);

  auto actions = game.legal_actions();
  std::sort(actions.begin(), actions.end(), order{});
  GOLV_LOG_DEBUG("actions = " << actions);

  auto [value, best_move] =  // mws_binary_search(game);
      mws_binary_search<golv::skat, order>(game, order{});
  GOLV_LOG_DEBUG("best_move = " << best_move);

  ASSERT_EQ(value, 34);
  ASSERT_EQ(best_move, "Ts");
}