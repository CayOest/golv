#include "trick.h"

#include <algorithm>
#include <cassert>
#include <iostream>

#include "game.h"
#include "rules.h"
#include "skatexception.h"
#include "state.h"
#include "util.h"

namespace skatomat {
Trick::Trick(const std::array<Player*, 3>& p) : players(p) {}

Trick Trick::clone(Game const& from, Game& to) const {
  auto const& fromPlayers = from.players();
  auto toPlayers = to.players();
  int rot = 0;
  if (players[0] == fromPlayers[1])
    rot = 1;
  else if (players[0] == fromPlayers[2])
    rot = 2;
  std::rotate(toPlayers.begin(), toPlayers.begin() + rot, toPlayers.end());
  Trick t(toPlayers);
  t.setCards(cards);
  t.setCurr(curr);
  return t;
}

Player* Trick::currentPlayer() const { return players[curr]; }

Player* Trick::advance(Card c) {
  players[curr]->remove(c);
  cards[curr] = c;
  ++curr;
  if (curr < 3)
    return players[curr];
  else
    return nullptr;
}

bool Trick::done() const { return curr >= 3; }

int Trick::size() const { return curr; }

void Trick::regress() {
  _REQUIRE(curr > 0, "curr < 0");
  --curr;
  players[curr]->add(cards[curr]);
}

int Trick::count() const {
  int sum = 0;
  for (int i = 0; i < curr; ++i) sum += eyes(cards[i]);
  return sum;
}

bool Trick::empty() const { return curr == 0; }

bool less_(const Card& first, const Card& other) {
  auto less = GameState::instance().less();
  if (isTrump(first)) {
    return less(first, other);
  } else {
    if (isTrump(other))
      return true;
    else if (first.suit() == other.suit())
      return less(first, other);
    else
      return false;
  }
}

Player* Trick::eval() const {
  _REQUIRE(curr == 3, "Trick not finished.");
  if (less_(cards[0], cards[1])) {
    if (less_(cards[1], cards[2]))
      return players[2];
    else
      return players[1];
  } else {
    if (less_(cards[0], cards[2]))
      return players[2];
    else
      return players[0];
  }
}

HandRange Trick::validCards() const {
  auto const& hand = players[curr]->hand();
  if (curr == 0) return {hand.begin(), hand.end()};

  Card const& first = cards[0];
  if (isTrump(first)) {
    auto it = std::find_if(hand.begin(), hand.end(), isTrump);
    if (it == hand.end()) it = hand.begin();
    return {it, hand.end()};
  } else {
    auto itTrump = std::find_if(hand.begin(), hand.end(), isTrump);
    auto itBeg = std::find_if(hand.begin(), itTrump, [&first](const Card& c) { return c.suit() == first.suit(); });
    if (itBeg == itTrump) return {hand.begin(), hand.end()};
    auto itEnd = std::find_if(itBeg, itTrump, [&first](const Card& c) { return c.suit() != first.suit(); });
    return {itBeg, itEnd};
  }
}
}  // namespace skatomat