#include "test_utils.hpp"

#include <algorithm>
#include <random>

golv::bridge create_random_game(size_t cards_per_suit, int rotate, unsigned seed) {
  golv::bridge game;
  std::array<golv::deck, 4> cards;
  golv::deck _deck = golv::create_deck(cards_per_suit);
  std::random_device rd;
  std::mt19937 g(rd());
  g.seed(seed);
  std::shuffle(_deck.begin(), _deck.end(), g);
  for (int i = 0; i < 4; ++i) {
    std::copy(_deck.begin() + (i * cards_per_suit), _deck.begin() + (i + 1) * cards_per_suit,
              std::back_inserter(cards[i]));
    std::sort(cards[i].begin(), cards[i].end());
  }
  std::rotate(cards.begin(), cards.begin() + rotate, cards.end());
  game.deal(cards);
  return game;
}