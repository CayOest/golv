#include <algorithm>
#include <cassert>
#include <golv/util/test_utils.hpp>
#include <random>

using namespace golv;

golv::bridge create_game() {
  bridge game;
  std::array<hand, 4> cards;
  auto d = create_bridge_deck();
  hand _deck = golv::create_bridge_deck();
  for (int i = 0; i < 4; ++i) {
    std::copy(_deck.begin() + (i * 13), _deck.begin() + (i + 1) * 13, std::back_inserter(cards[i]));
  }
  game.deal(cards);
  return game;
}

golv::bridge create_random_game(size_t cards_per_player, int rotate, unsigned seed) {
  bridge game;
  std::array<hand, 4> cards;
  auto _deck = golv::create_bridge_deck();
  std::random_device rd;
  std::mt19937 g(rd());
  if (seed != 0) g.seed(seed);
  std::shuffle(_deck.begin(), _deck.end(), g);
  for (size_t i = 0; i < golv::bridge::num_players; ++i) {
    std::copy(_deck.begin() + (i * cards_per_player), _deck.begin() + (i + 1) * cards_per_player,
              std::back_inserter(cards[i]));
    std::sort(cards[i].begin(), cards[i].end(), bridge_card_order{});
  }
  std::rotate(cards.begin(), cards.begin() + rotate, cards.end());
  game.deal(cards);
  return game;
}

golv::skat create_random_skat_game(size_t cards_per_player, int rotate, unsigned seed) {
  golv::skat game;
  std::array<hand, golv::skat::num_players + 1> cards;
  auto _deck = golv::create_skat_deck();
  std::random_device rd;
  std::mt19937 g(rd());
  if (seed != 0) g.seed(seed);
  std::shuffle(_deck.begin(), _deck.end(), g);
  for (size_t i = 0; i < golv::skat::num_players; ++i) {
    std::copy(_deck.begin() + (i * cards_per_player), _deck.begin() + (i + 1) * cards_per_player,
              std::back_inserter(cards[i]));
    std::sort(cards[i].begin(), cards[i].end(), golv::skat_card_order{});
  }
  auto N = golv::skat::num_players * cards_per_player;
  cards[golv::skat::num_players] = {_deck[N], _deck[N + 1]};
  std::rotate(cards.begin(), cards.begin() + rotate, cards.begin() + skat::num_players);
  game.deal(cards[0], cards[1], cards[2], cards[3]);
  game.set_soloist(0);
  game.skip_pushing();
  return game;
}