#include "test_utils.hpp"

#include <algorithm>
#include <cassert>
#include <random>

using namespace golv;

golv::bridge create_game(size_t cards_per_suit) {
  bridge game;
  std::array<deck, 4> cards;
  deck _deck = golv::create_deck(13);
  for (int i = 0; i < 4; ++i) {
    std::copy(_deck.begin() + (i * cards_per_suit), _deck.begin() + (i + 1) * cards_per_suit,
              std::back_inserter(cards[i]));
  }
  game.deal(cards);
  return game;
}

golv::bridge create_random_game(size_t cards_per_suit, int rotate, unsigned seed) {
  golv::bridge game;
  std::array<golv::deck, 4> cards;
  golv::deck _deck = golv::create_deck(cards_per_suit);
  std::random_device rd;
  std::mt19937 g(rd());
  g.seed(seed);
  std::shuffle(_deck.begin(), _deck.end(), g);
  for (size_t i = 0; i < golv::bridge::num_players; ++i) {
    std::copy(_deck.begin() + (i * cards_per_suit), _deck.begin() + (i + 1) * cards_per_suit,
              std::back_inserter(cards[i]));
    std::sort(cards[i].begin(), cards[i].end());
  }
  std::rotate(cards.begin(), cards.begin() + rotate, cards.end());
  game.deal(cards);
  return game;
}

golv::skat create_random_skat_game(size_t cards_per_player, int rotate, unsigned seed) {
  golv::skat game;
  std::array<golv::deck, golv::skat::num_players + 1> cards;
  golv::deck _deck = golv::create_deck(8);
  std::random_device rd;
  std::mt19937 g(rd());
  g.seed(seed);
  std::shuffle(_deck.begin(), _deck.end(), g);
  for (size_t i = 0; i < golv::skat::num_players; ++i) {
    std::copy(_deck.begin() + (i * cards_per_player), _deck.begin() + (i + 1) * cards_per_player,
              std::back_inserter(cards[i]));
    std::sort(cards[i].begin(), cards[i].end(), golv::skat_card_order{});
  }
  auto N = golv::skat::num_players * cards_per_player;
  cards[golv::skat::num_players] = {_deck[N], _deck[N + 1]};
  std::rotate(cards.begin(), cards.begin() + rotate, cards.begin() + skat::num_players);
  game.deal(cards);
  return game;
}