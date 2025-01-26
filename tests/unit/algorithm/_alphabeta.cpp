#include "golv/util/logging.hpp"
#include <gtest/gtest.h>

#include <golv/algorithm/alphabeta.hpp>
#include <golv/games/bridge.hpp>
#include <golv/games/connectfour.hpp>
#include <golv/games/tictactoe.hpp>

#include <algorithm>
#include <functional>
#include <random>

class _alphabeta : public ::testing::Test {
 protected:
  void SetUp() override { golv::set_log_level(golv::log_level::trace); }
};

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

namespace {

golv::bridge
create_random_game(size_t cards_per_suit = 13, int rotate = 0, unsigned seed = 91189)
{
    golv::bridge game;
    std::array<golv::deck, 4> cards;
    golv::deck _deck = golv::create_deck(cards_per_suit);
    std::random_device rd;
    std::mt19937 g(rd());
    g.seed(seed);
    std::shuffle(_deck.begin(), _deck.end(), g);
    for (int i = 0; i < 4; ++i) {
        std::copy(
          _deck.begin() + (i * cards_per_suit), _deck.begin() + (i + 1) * cards_per_suit, std::back_inserter(cards[i]));
        std::sort(cards[i].begin(), cards[i].end());
    }
    std::rotate(cards.begin(), cards.begin() + rotate, cards.end());
    game.deal(cards);
    return game;
}

}

TEST_F(_alphabeta, bridge_3cps) {
  auto game = create_random_game(3);
  GOLV_LOG_TRACE("game = " << game.state());

  auto [solution, best_move] = golv::alphabeta(game);
  ASSERT_EQ(solution, 3);
}

TEST_F(_alphabeta, bridge_5cps) {
  auto game = create_random_game(5);
  GOLV_LOG_DEBUG("game = " << game.state());

  auto [solution, best_move] = golv::alphabeta(game);
  GOLV_LOG_DEBUG("solution = " << solution << ", best_move = " << best_move);
  ASSERT_EQ(solution, 4);
}

TEST_F(_alphabeta, bridge_5cps_rot1) {
  auto game = create_random_game(5, 1);
  GOLV_LOG_DEBUG("game = " << game.state());

  auto [solution, best_move] = golv::alphabeta(game);
  GOLV_LOG_DEBUG("solution = " << solution << ", best_move = " << best_move);
  ASSERT_EQ(solution, 2);
}

TEST_F(_alphabeta, bridge_5cps_1) {
  auto game = create_random_game(5);
  game.set_soloist(1);

  GOLV_LOG_DEBUG("game = " << game.state());

  auto [solution, best_move] = golv::alphabeta(game);
  GOLV_LOG_DEBUG("solution = " << solution << ", best_move = " << best_move);
  ASSERT_EQ(solution, 1);
}

TEST_F(_alphabeta, bridge_5cps_2) {
  auto game = create_random_game(5);
  game.set_soloist(2);

  GOLV_LOG_DEBUG("game = " << game.state());

  auto [solution, best_move] = golv::alphabeta(game);
  GOLV_LOG_DEBUG("solution = " << solution << ", best_move = " << best_move);
  ASSERT_EQ(solution, 4);
}

TEST_F(_alphabeta, bridge_5cps_3) {
  auto game = create_random_game(5);
  game.set_soloist(3);

  GOLV_LOG_DEBUG("game = " << game.state());

  auto [solution, best_move] = golv::alphabeta(game);
  GOLV_LOG_DEBUG("solution = " << solution << ", best_move = " << best_move);
  ASSERT_EQ(solution, 1);
}

TEST_F(_alphabeta, bridge_3cps_with_memory) {
  auto game = create_random_game(3);
  GOLV_LOG_DEBUG("game = " << game.state());

  auto [solution, best_move] = golv::alphabeta_with_memory(game);
  GOLV_LOG_DEBUG("solution = " << solution << ", best_move = " << best_move);
  ASSERT_EQ(solution, 3);
}

TEST_F(_alphabeta, bridge_4cps_with_memory) {
  auto game = create_random_game(4);
  GOLV_LOG_DEBUG("game = " << game.state());

  auto [solution, best_move] = golv::alphabeta_with_memory(game);
  GOLV_LOG_DEBUG("solution = " << solution << ", best_move = " << best_move);
  ASSERT_EQ(solution, 4);
}

TEST_F(_alphabeta, bridge_5cps_with_memory) {
  auto game = create_random_game(5);
  GOLV_LOG_DEBUG("game = " << game.state());

  auto [solution, best_move] = golv::alphabeta_with_memory(game);
  GOLV_LOG_DEBUG("solution = " << solution << ", best_move = " << best_move);
  ASSERT_EQ(solution, 4);
}

TEST_F(_alphabeta, bridge_5cps_rot1_with_memory) {
  auto game = create_random_game(5, 1);
  GOLV_LOG_DEBUG("game = " << game.state());

  auto [solution, best_move] = golv::alphabeta_with_memory(game);
  GOLV_LOG_DEBUG("solution = " << solution << ", best_move = " << best_move);
  ASSERT_EQ(solution, 2);
}

TEST_F(_alphabeta, bridge_5cps_1_with_memory) {
  auto game = create_random_game(5);
  game.set_soloist(1);
  GOLV_LOG_DEBUG("game = " << game.state());

  auto [solution, best_move] = golv::alphabeta_with_memory(game);
  GOLV_LOG_DEBUG("solution = " << solution << ", best_move = " << best_move);
  ASSERT_EQ(solution, 1);
}

TEST_F(_alphabeta, bridge_5cps_2_with_memory) {
  auto game = create_random_game(5);
  game.set_soloist(2);
  GOLV_LOG_DEBUG("game = " << game.state());

  auto [solution, best_move] = golv::alphabeta_with_memory(game);
  GOLV_LOG_DEBUG("solution = " << solution << ", best_move = " << best_move);
  ASSERT_EQ(solution, 4);
}

TEST_F(_alphabeta, bridge_5cps_3_with_memory) {
  auto game = create_random_game(5);
  game.set_soloist(3);
  GOLV_LOG_DEBUG("game = " << game.state());

  auto [solution, best_move] = golv::alphabeta_with_memory(game);
  GOLV_LOG_DEBUG("solution = " << solution << ", best_move = " << best_move);
  ASSERT_EQ(solution, 1);
}