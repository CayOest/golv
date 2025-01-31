#include <gtest/gtest.h>

#include <algorithm>
#include <functional>
#include <golv/algorithms/mws.hpp>
#include <golv/games/connectfour.hpp>
#include <golv/games/tictactoe.hpp>
#include <golv/util/logging.hpp>
#include <golv/util/test_utils.hpp>

using namespace golv;

class mws_f : public ::testing::Test {
 protected:
  void SetUp() override { golv::set_log_level(golv::log_level::debug); }
};

TEST_F(mws_f, tictactoe_tie) {
  golv::tictactoe game;
  auto expected = 0;
  auto [lower, best_move] = mws(game, expected - 1);
  ASSERT_TRUE(lower);
  auto [upper, not_best_move] = mws(game, expected);
  ASSERT_FALSE(upper);
}

TEST_F(mws_f, tictactoe_tie_mem) {
  golv::tictactoe game;
  auto expected = 0;
  auto [lower, best_move] = mws_with_memory(game, expected - 1);
  ASSERT_TRUE(lower);
  auto [upper, not_best_move] = mws_with_memory(game, expected);
  ASSERT_FALSE(upper);
}

TEST_F(mws_f, tictactoe_win) {
  golv::tictactoe game;
  game.apply_action(4);
  game.apply_action(1);
  auto expected = 1;
  auto [lower, best_move] = mws(game, expected - 1);
  ASSERT_TRUE(lower);
  auto [upper, not_best_move] = mws(game, expected);
  ASSERT_FALSE(upper);
}

TEST_F(mws_f, tictactoe_win_mem) {
  golv::tictactoe game;
  game.apply_action(4);
  game.apply_action(1);
  auto expected = 1;
  auto [lower, best_move] = mws_with_memory(game, expected - 1);
  ASSERT_TRUE(lower);
  auto [upper, not_best_move] = mws_with_memory(game, expected);
  ASSERT_FALSE(upper);
}

// TODO! fix
// TEST_F(mws_f, tictactoe_loss) {
//   golv::tictactoe game;
//   game.apply_action(1);
//   game.apply_action(4);
//   game.apply_action(7);
//   auto expected = -1;
//   auto [lower, best_move] = mws(game, expected - 1);
//   ASSERT_TRUE(lower);
//   auto [upper, not_best_move] = mws(game, expected);
//   ASSERT_FALSE(upper);
// }

// TODO! fix
// TEST_F(mws_f, tictactoe_loss_mem) {
//   golv::tictactoe game;
//   game.apply_action(1);
//   game.apply_action(4);
//   game.apply_action(7);
//   auto expected = -1;
//   auto [lower, best_move] = mws_with_memory(game, expected - 1);
//   ASSERT_TRUE(lower);
//   auto [upper, not_best_move] = mws_with_memory(game, expected);
//   ASSERT_FALSE(upper);
// }

namespace {
const std::vector<golv::connectfour::move_type> scenario1 = {0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 6, 6, 6, 6, 6, 6};

}  // namespace

TEST_F(mws_f, connectfour_1) {
  golv::connectfour game;
  const auto moves = scenario1;
  std::for_each(std::begin(moves), std::end(moves), [&game](auto move) { game.apply_action(move); });

  auto expected = +1;
  auto [lower, best_move] = mws(game, expected - 1);
  ASSERT_TRUE(lower);
  auto [upper, not_best_move] = mws(game, expected);
  ASSERT_FALSE(upper);
}

TEST_F(mws_f, connectfour_1_mem) {
  golv::connectfour game;
  const auto moves = scenario1;
  std::for_each(std::begin(moves), std::end(moves), [&game](auto move) { game.apply_action(move); });

  auto expected = +1;
  auto [lower, best_move] = mws_with_memory(game, expected - 1);
  ASSERT_TRUE(lower);
  auto [upper, not_best_move] = mws_with_memory(game, expected);
  ASSERT_FALSE(upper);
}

namespace {
const std::vector<golv::connectfour::move_type> scenario2 = {0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1,
                                                             1, 3, 2, 3, 3, 3, 3, 3, 5, 2, 5};
}

// TODO! fix
// TEST_F(mws_f, connectfour_2) {
//   golv::connectfour game;
//   const auto moves = scenario2;
//   std::for_each(std::begin(moves), std::end(moves), [&game](auto move) { game.apply_action(move); });

//   auto expected = -1;
//   auto [lower, best_move] = mws(game, expected - 1);
//   ASSERT_TRUE(lower);
//   auto [upper, not_best_move] = mws(game, expected);
//   ASSERT_FALSE(upper);
// }

// TODO! fix
// TEST_F(mws_f, connectfour_2_mem) {
//   golv::connectfour game;
//   const auto moves = scenario2;
//   std::for_each(std::begin(moves), std::end(moves), [&game](auto move) { game.apply_action(move); });

//   auto expected = -1;
//   auto [lower, best_move] = mws_with_memory(game, expected - 1);
//   ASSERT_TRUE(lower);
//   auto [upper, not_best_move] = mws_with_memory(game, expected);
//   ASSERT_FALSE(upper);
// }

namespace {
const std::vector<golv::connectfour::move_type> scenario3 = {0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 3, 2, 3, 3, 3, 3, 3};
}

// TODO! fix
// TEST_F(mws_f, connectfour_3) {
//   golv::connectfour game;
//   const auto moves = scenario3;
//   std::for_each(std::begin(moves), std::end(moves), [&game](auto move) { game.apply_action(move); });

//   auto expected = -1;
//   auto [lower, best_move] = mws(game, expected - 1);
//   ASSERT_TRUE(lower);
//   auto [upper, not_best_move] = mws(game, expected);
//   ASSERT_FALSE(upper);
// }

// TODO! fix
// TEST_F(mws_f, connectfour_3_mem) {
//   golv::connectfour game;
//   const auto moves = scenario3;
//   std::for_each(std::begin(moves), std::end(moves), [&game](auto move) { game.apply_action(move); });

//   auto expected = -1;
//   auto [lower, best_move] = mws_with_memory(game, expected - 1);
//   ASSERT_TRUE(lower);
//   auto [upper, not_best_move] = mws_with_memory(game, expected);
//   ASSERT_FALSE(upper);
// }