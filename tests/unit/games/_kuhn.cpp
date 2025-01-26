#include <gtest/gtest.h>

#include <golv/games/kuhn.hpp>

using namespace golv;

bool state_ends_with(kuhn const& game, const std::string& suffix) {
  const auto& state = game.state();
  return state.size() >= suffix.size() && state.compare(state.size() - suffix.size(), suffix.size(), suffix) == 0;
}

TEST(kuhn_, initial_state) {
  kuhn game;
  EXPECT_TRUE(game.is_chance_node());
  EXPECT_FALSE(game.is_terminal());
  EXPECT_EQ(game.legal_actions(), "xb");
}

TEST(kuhn_, handle_chance_node) {
  kuhn game;
  game.handle_chance_node();
  EXPECT_FALSE(game.is_chance_node());
  EXPECT_EQ(game.current_player(), 0);
  EXPECT_EQ(game.legal_actions(), "xb");
  EXPECT_TRUE(game.state().find("|") != std::string::npos);  // State should include "|"
}

TEST(kuhn_, deal) {
  kuhn game;
  game.deal(2, 1);
  EXPECT_FALSE(game.is_chance_node());
  EXPECT_EQ(game.current_player(), 0);
  EXPECT_TRUE(game.state().find("|") != std::string::npos);  // State should include "|"
}

TEST(kuhn_, apply_action_check) {
  kuhn game;
  game.deal(2, 1);
  game.apply_action('x');
  EXPECT_TRUE(state_ends_with(game, "|x"));
  EXPECT_EQ(game.current_player(), 1);
  EXPECT_EQ(game.legal_actions(), "xb");
}

TEST(kuhn_, apply_action_bet) {
  kuhn game;
  game.deal(1, 0);
  game.apply_action('b');
  EXPECT_TRUE(state_ends_with(game, "|b"));
  EXPECT_EQ(game.current_player(), 1);
  EXPECT_EQ(game.legal_actions(), "fc");
}

TEST(kuhn_, terminal_state_xx) {
  kuhn game;
  game.deal(0, 1);
  game.apply_action('x');
  game.apply_action('x');
  EXPECT_TRUE(game.is_terminal());
  EXPECT_TRUE(state_ends_with(game, "|xx"));
  EXPECT_NO_THROW(game.value());
}

TEST(kuhn_, terminal_state_bc) {
  kuhn game;
  game.deal(1, 2);
  game.apply_action('b');
  game.apply_action('c');
  EXPECT_TRUE(game.is_terminal());
  EXPECT_TRUE(state_ends_with(game, "|bc"));
  EXPECT_NO_THROW(game.value());
}

TEST(kuhn_, terminal_state_bf) {
  kuhn game;
  game.deal(2, 0);
  game.apply_action('b');
  game.apply_action('f');
  EXPECT_TRUE(game.is_terminal());
  EXPECT_TRUE(state_ends_with(game, "|bf"));
  EXPECT_NO_THROW(game.value());
  EXPECT_EQ(game.value(), 1);  // Player 2 folded, Player 1 wins
}

TEST(kuhn_, undo_action) {
  kuhn game;
  game.deal(2, 1);
  game.apply_action('x');
  game.undo_action('x');
  EXPECT_TRUE(game.state().find("|") != std::string::npos);  // State includes "|"
  EXPECT_EQ(game.current_player(), 0);
}

TEST(kuhn_, invalid_action) {
  kuhn game;
  game.deal(0, 1);
  EXPECT_THROW(game.apply_action('z'), std::domain_error);
}

TEST(kuhn_, value_xx) {
  kuhn game;
  game.deal(2, 1);
  game.apply_action('x');
  game.apply_action('x');
  EXPECT_TRUE(game.is_terminal());
  EXPECT_EQ(game.value(), 1);  // Player 1 wins with higher card
}

TEST(kuhn_, value_bc) {
  kuhn game;
  game.deal(0, 1);
  game.apply_action('b');
  game.apply_action('c');
  EXPECT_TRUE(game.is_terminal());
  EXPECT_EQ(game.value(), -2);  // Player 2 wins with higher card
}

TEST(kuhn_, value_all) {
  kuhn game;
  auto test_value_calculation = [&](int card1, int card2, const std::string& actions, int max_player,
                                    double expected_value) {
    game.reset();
    game.deal(card1, card2);
    game.set_max(max_player);

    for (char action : actions) {
      game.apply_action(action);
    }

    ASSERT_TRUE(game.is_terminal());
    EXPECT_EQ(game.value(), expected_value)
        << "Failed with cards: " << card1 << ", " << card2 << " actions: " << actions << " max_player: " << max_player;
  };

  // Test all card combinations and action sequences
  for (int card1 = 0; card1 <= 2; ++card1) {
    for (int card2 = 0; card2 <= 2; ++card2) {
      if (card1 == card2) continue;  // Skip invalid combinations

      // Test with max_player = 0
      test_value_calculation(card1, card2, "xx", 0, card1 > card2 ? 1 : -1);  // Both check
      test_value_calculation(card1, card2, "bf", 0, +1);
      test_value_calculation(card1, card2, "xbf", 0, -1);
      test_value_calculation(card1, card2, "bc", 0, card1 > card2 ? 2 : -2);
      test_value_calculation(card1, card2, "xbc", 0, card1 > card2 ? 2 : -2);

      // Test with max_player = 1
      test_value_calculation(card1, card2, "xx", 1, card1 > card2 ? -1 : 1);  // Both check
      test_value_calculation(card1, card2, "bf", 1, -1);
      test_value_calculation(card1, card2, "xbf", 1, +1);  // Player 1 folds
      test_value_calculation(card1, card2, "bc", 1, card1 > card2 ? -2 : +2);
      test_value_calculation(card1, card2, "xbc", 1, card1 > card2 ? -2 : 2);  // Player 1 calls
    }
  }
}