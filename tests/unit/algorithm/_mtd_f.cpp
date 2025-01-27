#include <gtest/gtest.h>

#include <algorithm>
#include <functional>
#include <golv/algorithm/mtd_f.hpp>
#include <golv/games/connectfour.hpp>
#include <golv/games/tictactoe.hpp>
#include <golv/util/test_utils.hpp>

using namespace golv;

class _mtd_f : public ::testing::Test {
 protected:
  void SetUp() override { golv::set_log_level(golv::log_level::debug); }
};

TEST_F(_mtd_f, tictactoe_tie) {
  golv::tictactoe game;
  auto solution = mtd_f(game).solve(0);
  ASSERT_EQ(solution, 0);
}

TEST_F(_mtd_f, tictactoe_win) {
  golv::tictactoe game;
  game.apply_action(4);
  game.apply_action(1);
  auto solution = mtd_f(game).solve(0);
  ASSERT_EQ(solution, 1);
}

TEST_F(_mtd_f, tictactoe_loss) {
  golv::tictactoe game;
  game.apply_action(1);
  game.apply_action(4);
  game.apply_action(7);
  auto solution = mtd_f(game).solve(0);
  ASSERT_EQ(solution, -1);
}

namespace {
auto ordering = [](golv::tictactoe::move_type left, golv::tictactoe::move_type right) {
    if (left == 4)
        return true;
    else {
        if (right == 4)
            return false;
        else
            return left == 1 || left == 3 || left == 5 || left == 7;
    }
};

auto connectfour_ordering = [](golv::connectfour::move_type left, golv::connectfour::move_type right) {
    auto dist_left = std::abs(static_cast<int>(left) - static_cast<int>(golv::connectfour::width / 2));
    auto dist_right = std::abs(static_cast<int>(right) - static_cast<int>(golv::connectfour::width / 2));
    return dist_left < dist_right;
};

const std::vector<golv::connectfour::move_type> scenario1 = { 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 6, 6, 6, 6, 6, 6 };

} // namespace

TEST_F(_mtd_f, connectfour_1) {
  golv::connectfour game;
  const auto moves = scenario1;
  std::for_each(std::begin(moves), std::end(moves), [&game](auto move) { game.apply_action(move); });

  auto solution = golv::mtd_f(game).solve(0);
  ASSERT_EQ(solution, +1);
}

namespace {
const std::vector<golv::connectfour::move_type> scenario2 = { 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1,
                                                              1, 3, 2, 3, 3, 3, 3, 3, 5, 2, 5 };
}

TEST_F(_mtd_f, connectfour_2) {
  golv::connectfour game;
  const auto moves = scenario2;
  std::for_each(std::begin(moves), std::end(moves), [&game](auto move) { game.apply_action(move); });

  auto solution = golv::mtd_f(game).solve(-1);
  ASSERT_EQ(solution, -1);
}

namespace {
const std::vector<golv::connectfour::move_type> scenario3 = { 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 3, 2, 3, 3, 3, 3, 3 };
}

TEST_F(_mtd_f, connectfour_3) {
  golv::connectfour game;
  const auto moves = scenario3;
  std::for_each(std::begin(moves), std::end(moves), [&game](auto move) { game.apply_action(move); });

  auto solution = golv::mtd_f(game).solve(0);
  ASSERT_EQ(solution, -1);
}

TEST_F(_mtd_f, bridge_5cps) {
  auto game = create_random_game(5);
  GOLV_LOG_DEBUG("game = " << game.state());

  int first_guess = 7;
  auto solution = golv::mtd_f(game).solve(first_guess);
  GOLV_LOG_DEBUG("solution = " << solution);
  ASSERT_EQ(solution, 4);
}

TEST_F(_mtd_f, bridge_5cps_rot1_with_memory) {
  auto game = create_random_game(5, 1);
  GOLV_LOG_DEBUG("game = " << game.state());

  int first_guess = 7;
  auto solution = golv::mtd_f(game).solve(first_guess);
  GOLV_LOG_DEBUG("solution = " << solution);
  ASSERT_EQ(solution, 2);
}

TEST_F(_mtd_f, bridge_5cps_1_with_memory) {
  auto game = create_random_game(5);
  game.set_soloist(1);
  GOLV_LOG_DEBUG("game = " << game.state());

  int first_guess = 7;
  auto solution = golv::mtd_f(game).solve(first_guess);
  GOLV_LOG_DEBUG("solution = " << solution);
  ASSERT_EQ(solution, 1);
}

TEST_F(_mtd_f, bridge_5cps_2_with_memory) {
  auto game = create_random_game(5);
  game.set_soloist(2);
  GOLV_LOG_DEBUG("game = " << game.state());

  int first_guess = 7;
  auto solution = golv::mtd_f(game).solve(first_guess);
  GOLV_LOG_DEBUG("solution = " << solution);
  ASSERT_EQ(solution, 4);
}

TEST_F(_mtd_f, bridge_5cps_3_with_memory) {
  auto game = create_random_game(5);
  game.set_soloist(3);
  GOLV_LOG_DEBUG("game = " << game.state());

  int first_guess = 7;
  auto solution = golv::mtd_f(game).solve(first_guess);
  GOLV_LOG_DEBUG("solution = " << solution);
  ASSERT_EQ(solution, 1);
}

TEST_F(_mtd_f, skat_9cards_with_mem) {
  golv::skat game = create_random_skat_game(9, 2);
  GOLV_LOG_DEBUG("game = " << game.state());
  auto solution = golv::mtd_f(game).solve(0);
  ASSERT_EQ(solution, 5);
}