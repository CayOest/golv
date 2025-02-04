#include <gtest/gtest.h>

#include <algorithm>
#include <functional>
#include <golv/algorithms/alphabeta.hpp>
#include <golv/games/bridge.hpp>
#include <golv/games/connectfour.hpp>
#include <golv/games/skat.hpp>
#include <golv/games/tictactoe.hpp>
#include <golv/util/logging.hpp>
#include <golv/util/test_utils.hpp>
#include <random>

#include "../util/test_games.hpp"

class _alphabeta : public ::testing::Test {
 protected:
  void SetUp() override { golv::set_log_level(golv::log_level::debug); }
};

namespace {
template <typename GameT>
void generate_best_move_sequence(GameT g, int num_moves) {
  int count = -1;
  while (!g.is_terminal()) {
    if ((++count % num_moves) == 0) {
      GOLV_LOG_DEBUG("game = " << g.state());
    }
    auto [solution, best_move] = golv::alphabeta_with_memory(g);
    GOLV_LOG_DEBUG("solution = " << solution << ", best_move = " << best_move);
    g.apply_action(best_move);
  }
}
}  // namespace

TEST_F(_alphabeta, tictactoe_tie) {
  golv::tictactoe game;
  auto [solution, best_move] = alphabeta(game);
  ASSERT_EQ(solution, 0);
}

TEST_F(_alphabeta, tictactoe_win) {
  golv::tictactoe game;
  game.apply_action(4);
  game.apply_action(1);
  auto [solution, best_move] = alphabeta(game);
  ASSERT_EQ(solution, 1);
}

TEST_F(_alphabeta, tictactoe_loss) {
  golv::tictactoe game;
  game.apply_action(1);
  game.apply_action(4);
  game.apply_action(7);
  auto [solution, best_move] = alphabeta(game);
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
}

TEST_F(_alphabeta, tictactoe_win_ordered) {
  golv::tictactoe game;
  game.apply_action(4);
  game.apply_action(1);
  auto [solution, best_move] = alphabeta(game, ordering);
  ASSERT_EQ(solution, 1);
}

TEST_F(_alphabeta, tictactoe_loss_ordered) {
  golv::tictactoe game;
  game.apply_action(1);
  game.apply_action(4);
  game.apply_action(7);
  auto [solution, best_move] = alphabeta(game, ordering);
  ASSERT_EQ(solution, -1);
}

TEST_F(_alphabeta, tictactoe_with_memory) {
  golv::tictactoe game;
  auto [solution, best_move] =
      golv::alphabeta(game, std::less<golv::tictactoe::move_type>{}, golv::unordered_table<golv::tictactoe>{});
  ASSERT_EQ(solution, 0);
}

namespace {
auto connectfour_ordering = [](golv::connectfour::move_type left, golv::connectfour::move_type right) {
    auto dist_left = std::abs(static_cast<int>(left) - static_cast<int>(golv::connectfour::width / 2));
    auto dist_right = std::abs(static_cast<int>(right) - static_cast<int>(golv::connectfour::width / 2));
    return dist_left < dist_right;
};

const std::vector<golv::connectfour::move_type> scenario1 = { 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 6, 6, 6, 6, 6, 6 };

} // namespace

TEST_F(_alphabeta, connectfour_raw) {
  golv::connectfour game;
  const auto moves = scenario1;
  std::for_each(std::begin(moves), std::end(moves), [&game](auto move) { game.apply_action(move); });

  auto [solution, best_move] = golv::alphabeta(game);
  ASSERT_EQ(solution, +1);
}

TEST_F(_alphabeta, connectfour_ordered) {
  golv::connectfour game;
  const auto moves = scenario1;
  std::for_each(std::begin(moves), std::end(moves), [&game](auto move) { game.apply_action(move); });

  auto [solution, best_move] = golv::alphabeta(game, connectfour_ordering);
  ASSERT_EQ(solution, +1);
}

TEST_F(_alphabeta, connectfour_ordered_lookup) {
  golv::connectfour game;
  const auto moves = scenario1;
  std::for_each(std::begin(moves), std::end(moves), [&game](auto move) { game.apply_action(move); });

  auto [solution, best_move] = golv::alphabeta(game, connectfour_ordering, golv::unordered_table<golv::connectfour>{});
  ASSERT_EQ(solution, +1);
}

namespace {
const std::vector<golv::connectfour::move_type> scenario2 = { 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1,
                                                              1, 3, 2, 3, 3, 3, 3, 3, 5, 2, 5 };
}

TEST_F(_alphabeta, connectfour_ordered_2) {
  golv::connectfour game;
  const auto moves = scenario2;
  std::for_each(std::begin(moves), std::end(moves), [&game](auto move) { game.apply_action(move); });

  auto [solution, best_move] = golv::alphabeta(game, connectfour_ordering);
  ASSERT_EQ(solution, -1);
}

TEST_F(_alphabeta, connectfour_ordered_lookup_2) {
  golv::connectfour game;
  const auto moves = scenario2;
  std::for_each(std::begin(moves), std::end(moves), [&game](auto move) { game.apply_action(move); });

  auto [solution, best_move] = golv::alphabeta(game, connectfour_ordering, golv::unordered_table<golv::connectfour>{});
  ASSERT_EQ(solution, -1);
}

TEST_F(_alphabeta, connectfour_unordered_lookup_2) {
  golv::connectfour game;
  const auto moves = scenario2;
  std::for_each(std::begin(moves), std::end(moves), [&game](auto move) { game.apply_action(move); });

  auto [solution, best_move] = golv::alphabeta_with_memory(game);
  ASSERT_EQ(solution, -1);
}

TEST_F(_alphabeta, bridge_3cps) {
  auto game = default_game_3();
  GOLV_LOG_DEBUG("game = " << game.state());

  auto [solution, best_move] = golv::alphabeta(game);
  ASSERT_EQ(solution, 2);
}

TEST_F(_alphabeta, bridge_5cps) {
  auto game = default_game_5();
  GOLV_LOG_DEBUG("game = " << game.state());

  auto [solution, best_move] = golv::alphabeta(game);
  GOLV_LOG_DEBUG("solution = " << solution << ", best_move = " << best_move);
  ASSERT_EQ(solution, 4);
}

TEST_F(_alphabeta, bridge_5cps_rot1) {
  auto game = default_game_5(1);
  GOLV_LOG_DEBUG("game = " << game.state());

  auto [solution, best_move] = golv::alphabeta(game);
  GOLV_LOG_DEBUG("solution = " << solution << ", best_move = " << best_move);
  ASSERT_EQ(solution, 5);
  generate_best_move_sequence(game, 4);
}

TEST_F(_alphabeta, bridge_5cps_1) {
  auto game = default_game_5(2);
  game.set_soloist(1);

  GOLV_LOG_DEBUG("game = " << game.state());

  auto [solution, best_move] = golv::alphabeta(game);
  GOLV_LOG_DEBUG("solution = " << solution << ", best_move = " << best_move);
  ASSERT_EQ(solution, 1);
}

TEST_F(_alphabeta, bridge_5cps_2) {
  auto game = default_game_5();
  game.set_soloist(2);

  GOLV_LOG_DEBUG("game = " << game.state());

  auto [solution, best_move] = golv::alphabeta(game);
  GOLV_LOG_DEBUG("solution = " << solution << ", best_move = " << best_move);
  ASSERT_EQ(solution, 4);
}

TEST_F(_alphabeta, bridge_5cps_3) {
  auto game = default_game_5();
  game.set_soloist(3);

  GOLV_LOG_DEBUG("game = " << game.state());

  auto [solution, best_move] = golv::alphabeta(game);
  GOLV_LOG_DEBUG("solution = " << solution << ", best_move = " << best_move);
  ASSERT_EQ(solution, 1);
}

TEST_F(_alphabeta, bridge_3cps_with_memory) {
  auto game = default_game_3();
  GOLV_LOG_DEBUG("game = " << game.state());

  auto [solution, best_move] = golv::alphabeta_with_memory(game);
  GOLV_LOG_DEBUG("solution = " << solution << ", best_move = " << best_move);
  ASSERT_EQ(solution, 2);
}

TEST_F(_alphabeta, bridge_5cps_with_memory) {
  auto game = default_game_5();
  GOLV_LOG_DEBUG("game = " << game.state());

  auto [solution, best_move] = golv::alphabeta_with_memory(game);
  GOLV_LOG_DEBUG("solution = " << solution << ", best_move = " << best_move);
  ASSERT_EQ(solution, 4);
}

TEST_F(_alphabeta, bridge_5cps_rot1_with_memory) {
  auto game = default_game_5(1);
  GOLV_LOG_DEBUG("game = " << game.state());

  auto [solution, best_move] = golv::alphabeta_with_memory(game);
  GOLV_LOG_DEBUG("solution = " << solution << ", best_move = " << best_move);
  ASSERT_EQ(solution, 5);
}

TEST_F(_alphabeta, bridge_5cps_1_with_memory) {
  auto game = default_game_5();
  game.set_soloist(1);
  GOLV_LOG_DEBUG("game = " << game.state());

  auto [solution, best_move] = golv::alphabeta_with_memory(game);
  GOLV_LOG_DEBUG("solution = " << solution << ", best_move = " << best_move);
  ASSERT_EQ(solution, 1);
}

TEST_F(_alphabeta, bridge_5cps_2_with_memory) {
  auto game = default_game_5();
  game.set_soloist(2);
  GOLV_LOG_DEBUG("game = " << game.state());

  auto [solution, best_move] = golv::alphabeta_with_memory(game);
  GOLV_LOG_DEBUG("solution = " << solution << ", best_move = " << best_move);
  ASSERT_EQ(solution, 4);
}

TEST_F(_alphabeta, bridge_5cps_3_with_memory) {
  auto game = default_game_5();
  game.set_soloist(3);
  GOLV_LOG_DEBUG("game = " << game.state());

  auto [solution, best_move] = golv::alphabeta_with_memory(game);
  GOLV_LOG_DEBUG("solution = " << solution << ", best_move = " << best_move);
  ASSERT_EQ(solution, 1);
}

TEST_F(_alphabeta, skat_5cards) {
  golv::skat game = default_skat_game_5();
  GOLV_LOG_DEBUG("game = " << game);

  auto [solution, best_move] = golv::alphabeta(game);
  ASSERT_EQ(solution, 59);
  golv::card expected{golv::kind::ten, golv::suit::hearts};
  ASSERT_EQ(best_move, expected);
}

TEST_F(_alphabeta, skat_5cards_rot1) {
  golv::skat game = default_skat_game_5(1);
  GOLV_LOG_DEBUG("game = " << game);
  auto [solution, best_move] = golv::alphabeta(game);
  ASSERT_EQ(solution, 0);
}

TEST_F(_alphabeta, skat_5cards_rot2) {
  golv::skat game = default_skat_game_5(2);
  GOLV_LOG_DEBUG("game = " << game);
  auto [solution, best_move] = golv::alphabeta(game);
  ASSERT_EQ(solution, 11);
  golv::card expected{golv::kind::ace, golv::suit::diamonds};
  ASSERT_EQ(best_move, expected);
}

TEST_F(_alphabeta, skat_7cards) {
  golv::skat game = default_skat_game_7(1);
  GOLV_LOG_DEBUG("game = " << game);
  // auto [solution, best_move] = golv::alphabeta_with_memory(game);
  auto [solution, best_move] = golv::alphabeta(game);
  ASSERT_EQ(solution, 17);
  golv::card expected = "8s";
  ASSERT_EQ(best_move, expected);
}

TEST_F(_alphabeta, skat_7cards_with_mem) {
  golv::skat game = default_skat_game_7(1);
  GOLV_LOG_DEBUG("game = " << game);
  auto [solution, best_move] = golv::alphabeta_with_memory(game);
  ASSERT_EQ(solution, 17);
  golv::card expected = "8s";
  ASSERT_EQ(best_move, expected);
}