#include "util.h"

#include <algorithm>
#include <numeric>

#include "state.h"

namespace skatomat {
Deck makeDeck() {
  Deck deck;
  int i = 0;
  for (auto const& suit : allSuits) {
    for (auto const& kind : allKinds) {
      deck[i++] = Card{kind, suit};
    }
  }
  return deck;
}

std::mt19937 shuffle(Deck& deck, unsigned seed) {
  std::mt19937 gen(seed);
  std::shuffle(deck.begin(), deck.end(), gen);
  return gen;
}

void shuffle(Deck& deck, std::mt19937 gen) { std::shuffle(deck.begin(), deck.end(), gen); }

int eyes(Card c) {
  if (c.kind() >= Kind::Jack) return static_cast<int>(c.kind());
  return 0;
}

int eyes(Hand const& h) {
  int result = 0;
  for (auto const& c : h) result += eyes(c);
  return result;
}

void sort(Hand& h) { std::sort(h.begin(), h.end(), GameState::instance().less()); }

bool isTrump(const Card& c) {
  auto const& t = GameState::instance().trump();
  if (t == Trump::Null) return false;
  if (t == Trump::Grand) return c.kind() == Kind::Jack;
  return c.kind() == Kind::Jack || static_cast<int>(c.suit()) == static_cast<int>(t);
}
}  // namespace skatomat