#include <gtest/gtest.h>

#include <algorithm>
#include <golv/games/bridge.hpp>
#include <golv/util/exception.hpp>
#include <golv/util/logging.hpp>
#include <golv/util/test_utils.hpp>
#include <iterator>
#include <random>

#include "../util/test_games.hpp"

using namespace golv;

TEST(bridge, create_deck)
{
  constexpr size_t cards_per_suit = 5;
  auto deck = create_deck<cards_per_suit>();
  ASSERT_EQ(deck.size(), cards_per_suit * 4);

  deck = create_deck<13>();
  ASSERT_EQ(deck.size(), 13 * 4);
}

TEST(bridge, card_order) {
  bridge_card_order less{suit::spades};

  // lead suit
  card As = "As";
  card Ks = "Ks";
  ASSERT_TRUE(less(Ks, As));

  // same suit != lead suit
  card Ac = "Ac";
  card Kc = "Kc";
  ASSERT_TRUE(less(Kc, Ac));

  // different suits != lead_suit
  card Ah = "Ah";
  ASSERT_TRUE(less(Ac, Ah));

  // lead suit vs. non-lead suit
  ASSERT_TRUE(less(Ac, Ks));
}

TEST(bridge, state)
{
    auto game = create_game();
    auto state = game.state();
    ASSERT_TRUE(state[0] == 'A' && state[1] == 's');
    ASSERT_TRUE(state[state.size() - 6] == '2' && state[state.size() - 5] == 'c');
    ASSERT_TRUE(state[state.size() - 1] == '0');
}

TEST(bridge, apply_action)
{
    auto game = create_game();
    game.apply_action("As");
    auto state = game.state();
    ASSERT_EQ(state[0], 'A');
    ASSERT_EQ(state[1], 's');
    ASSERT_EQ(game.current_player(), 1);
}

TEST(bridge, apply_action_4)
{
    auto game = create_game();
    game.apply_action({ kind::ace, suit::spades });
    auto state = game.state();
    ASSERT_EQ(game.tricks().size(), 1);
    ASSERT_TRUE(state[0] == 'A' && state[1] == 's');
    ASSERT_EQ(game.current_player(), 1);
    game.apply_action({ kind::ace, suit::hearts });
    ASSERT_EQ(game.current_player(), 2);
    game.apply_action({ kind::ace, suit::diamonds });
    ASSERT_EQ(game.current_player(), 3);
    game.apply_action({ kind::ace, suit::clubs });
    ASSERT_EQ(game.current_player(), 0);
    ASSERT_EQ(game.tricks().size(), 2);
}

TEST(bridge, legal_actions)
{
  auto game = default_game_13();
  GOLV_LOG_DEBUG("" << game.state());
  game.apply_action({kind::king, suit::spades});
  GOLV_LOG_DEBUG(game.state());
  ASSERT_EQ(game.value(), 0);
  auto moves2 = game.legal_actions();
  ASSERT_EQ(moves2.size(), 5);
  game.apply_action(moves2.front());
  ASSERT_EQ(game.value(), 0);
  auto moves3 = game.legal_actions();
  ASSERT_EQ(moves3.size(), 4);
  game.apply_action(moves3.front());
  ASSERT_EQ(game.value(), 0);
  auto moves4 = game.legal_actions();
  ASSERT_EQ(moves4.size(), 2);
  game.apply_action(moves4.front());
  ASSERT_EQ(game.value(), 1);
  ASSERT_EQ(game.tricks().size(), 2);
  ASSERT_EQ(game.current_player(), 0);
  //------------------------------------
  GOLV_LOG_DEBUG("" << game.state());
  auto moves5 = game.legal_actions();
  ASSERT_EQ(moves5.size(), 12);
  game.apply_action(moves5.back());
  ASSERT_EQ(game.value(), 0);
  auto moves6 = game.legal_actions();
  ASSERT_EQ(moves6.size(), 4);
  game.apply_action(moves6.front());
  ASSERT_EQ(game.value(), 0);
  auto moves7 = game.legal_actions();
  ASSERT_EQ(moves7.size(), 3);
  game.apply_action(moves7.back());
  ASSERT_EQ(game.value(), 0);
  auto moves8 = game.legal_actions();
  ASSERT_EQ(moves8.size(), 1);
  game.apply_action(moves8.back());
  ASSERT_EQ(game.value(), 1);
  ASSERT_EQ(game.tricks().size(), 3);
  ASSERT_EQ(game.current_player(), 2);
}

TEST(bridge, terminal)
{
    constexpr size_t cards_per_suit = 3;
    auto game = default_game_3();
    for (size_t i = 1; i < cards_per_suit * 4; ++i) {
        auto moves = game.legal_actions();
        game.apply_action(moves.front());
        ASSERT_FALSE(game.is_terminal());
    }
    game.apply_action(game.legal_actions().front());
    ASSERT_TRUE(game.is_terminal());
}

TEST(bridge, undo_action)
{
  auto game = default_game_13();
  auto move = game.legal_actions().front();
  game.apply_action(move);
  ASSERT_EQ(game.current_player(), 1);
  game.undo_action(move);
  ASSERT_EQ(game.current_player(), 0);
}

TEST(bridge, undo_action_after_trick)
{
  auto game = default_game_13();
  GOLV_LOG_DEBUG("" << game.state());
  std::vector<bridge::move_type> moves;
  for (int i = 0; i < 4; ++i) {
    moves.push_back(game.legal_actions().front());
    game.apply_action(moves.back());
  }
    ASSERT_EQ(game.current_player(), 0);
    game.undo_action(moves.back());
    ASSERT_EQ(game.current_player(), 3);
    game.apply_action(moves.back());
    ASSERT_EQ(game.current_player(), 0);
    ASSERT_EQ(game.tricks().size(), 2);
}

TEST(bridge, undo)
{
  auto game = default_game_3();
  GOLV_LOG_DEBUG("" << game.state());
  std::vector<bridge::move_type> moves;
  for (int i = 0; i < 3; ++i) {
    for (int i = 0; i < 4; ++i) {
      moves.push_back(game.legal_actions().front());
      game.apply_action(moves.back());
    }
  }
    ASSERT_TRUE(game.is_terminal());
    game.undo_action(moves.back());
    moves.pop_back();
    ASSERT_EQ(game.legal_actions().size(), 1);
    game.undo_action(moves.back());
    moves.pop_back();
    ASSERT_EQ(game.legal_actions().size(), 1);
    game.undo_action(moves.back());
    moves.pop_back();
    ASSERT_EQ(game.legal_actions().size(), 1);
    game.undo_action(moves.back());
    moves.pop_back();
    GOLV_LOG_DEBUG("" << game.state());
    ASSERT_EQ(game.legal_actions().size(), 1);
    game.undo_action(moves.back());
    moves.pop_back();
    ASSERT_EQ(game.legal_actions().size(), 2);
    game.undo_action(moves.back());
    moves.pop_back();
    ASSERT_EQ(game.legal_actions().size(), 2);
    game.undo_action(moves.back());
    moves.pop_back();
    ASSERT_EQ(game.legal_actions().size(), 2);
    game.undo_action(moves.back());
    moves.pop_back();
    GOLV_LOG_DEBUG("" << game.state());
    ASSERT_EQ(game.legal_actions().size(), 2);
    game.undo_action(moves.back());
    moves.pop_back();
    ASSERT_EQ(game.legal_actions().size(), 1);
    game.undo_action(moves.back());
    moves.pop_back();
    ASSERT_EQ(game.legal_actions().size(), 1);
    game.undo_action(moves.back());
    moves.pop_back();
    ASSERT_EQ(game.legal_actions().size(), 3);
    game.undo_action(moves.back());
    moves.pop_back();
    ASSERT_EQ(game.legal_actions().size(), 3);
    ASSERT_TRUE(game.tricks().size() == 1 && game.tricks().back().cards_.empty());
}