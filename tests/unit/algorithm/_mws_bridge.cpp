#include <gtest/gtest.h>
#include <skatomat2/game.h>
#include <skatomat2/state.h>
#include <skatomat2/util.h>

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

struct order {
  skat_card_order skat_order{suit::clubs};
  bool operator()(const card& left, const card& right) {          //
    return !operator==(left, right) && !skat_order(left, right);  //
  }
};

TEST_F(mws_bridge, skat_8cps_with_mem) {
  auto game = create_random_skat_game(8, 2);
  GOLV_LOG_DEBUG("game = " << game);

  auto [value, best_move] = mws_binary_search<golv::skat, order>(game, order{});

  ASSERT_EQ(value, 32);
}

TEST_F(mws_bridge, skat_10cps_with_mem) {
  auto game = default_skat_game_10();
  golv::experimental::log_debug("game = ", game);
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

TEST_F(mws_bridge, skat_10cps_with_mem_rot1) {
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

TEST_F(mws_bridge, skat_10cps_with_mem_rot2) {
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

using namespace skatomat;

namespace {
class skatomat_game_wrapper {
 public:
  using value_type = short;
  using state_type = uint64_t;
  using move_type = skatomat::Card;
  using move_range = std::vector<move_type>;
  using player_type = short;

  skatomat_game_wrapper() = default;

  skatomat_game_wrapper(const skatomat::Game& game) : game_(game) {}

  value_type value() const { return game_.counts().first; }
  bool is_terminal() const {
    auto valid = game_.validCards();
    return valid.first >= valid.second;
  }
  bool hash_me() const { return game_.currentIsLead(); }
  state_type state() { return game_.getState(); }
  void apply_action(move_type a) { game_.advance(a); }
  void undo_action(move_type /*a*/) { game_.regress(); }
  move_range legal_actions() {
    auto valid = game_.validCards();
    return move_range{valid.first, valid.second};
  }
  bool is_max() const { return game_.currentIsSingle(); }

 private:
  skatomat::Game game_;
};
}  // namespace

TEST(mws_skatomat, test) {
  skatomat::GameState::instance().setTrump(Trump::Grand);
  skatomat::GameState::instance().setLogLevel(LogLevel::None);

  Deck d = makeDeck();
  unsigned seed = 31280;
  shuffle(d, seed);
  const int numCards = 10;
  // GOLV_LOG_DEBUG("deck = " << d.size());
  // for (int i = 0; i < d.size(); ++i) {
  //   GOLV_LOG_DEBUG("" << d[i]);
  // }
  Hand cards_fh = skatomat::toHand("9d Td 8h 9h Th 9s Ks 7c 9c Ac");
  Player fh("FH", cards_fh);
  Hand cards_mh = skatomat::toHand("8d Kd Ad 7s 8s Qs 8c Tc Jh Jc");
  Player mh("MH", cards_mh);
  Hand cards_bh = skatomat::toHand("Qd 7h Qh Kh Ah As Qc Kc Jd Js");
  Player bh("BH", cards_bh);
  GOLV_LOG_DEBUG("start");
  std::cout << "FH = " << fh.hand() << std::endl;
  std::cout << "MH = " << mh.hand() << std::endl;
  std::cout << "BH = " << bh.hand() << std::endl;
  // Hand rest = {d.begin() + numCards, d.begin() + 10};
  // rest.insert(rest.end(), d.begin() + 10 + numCards, d.begin() + 20);
  // rest.insert(rest.end(), d.begin() + 20 + numCards, d.end());
  // int restEyes = eyes(rest);

  skatomat::Game g(fh, mh, bh);
  g.setSinglePlayer("FH");
  g.setSkat(skatomat::toHand("7d Ts"));
  skatomat_game_wrapper wrapper(g);
  auto solution = golv::mws_binary_search(wrapper);
  GOLV_LOG_DEBUG("solution = " << solution.first << " " << solution.second);
}