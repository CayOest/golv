#include "game.h"

#include <algorithm>
#include <iostream>
#include <sstream>

#include "skatexception.h"
#include "state.h"

namespace skatomat {
Game::Game(Player const& one, Player const& two, Player const& three)
    : players_{one, two, three}, single_{&players_[0]} {
  nextTrick();
}

Game::Game(const Game& g) : players_{g.players_} {
  if (g.single_ == &g.players_[0])
    single_ = &players_[0];
  else if (g.single_ == &g.players_[1])
    single_ = &players_[1];
  else if (g.single_ == &g.players_[2])
    single_ = &players_[2];
  else
    _FAIL("No single player.");

  for (auto const& t : g.tricks_) {
    tricks_.push_back(t.clone(g, *this));
  }

  singleCount_ = g.singleCount_;
  oppoCount_ = g.oppoCount_;
  skat_ = g.skat_;
}

namespace {
std::array<Player*, 3> getOrder(Player* winner, std::array<Player*, 3> p) {
  int newForehand = 0;
  if (winner == p[1])
    newForehand = 1;
  else if (winner == p[2])
    newForehand = 2;
  std::rotate(p.begin(), p.begin() + newForehand, p.end());
  return p;
}
}  // namespace

Trick& Game::currentTrick() {
  _REQUIRE(!tricks_.empty(), "No trick yet.");
  return tricks_.back();
}

Trick const& Game::currentTrick() const {
  _REQUIRE(!tricks_.empty(), "No trick yet.");
  return tricks_.back();
}

size_t Game::trickCount() const { return tricks_.size(); }

std::pair<int, int> Game::counts() const { return {singleCount_, oppoCount_}; }

Player* Game::nextTrick() {
  if (tricks_.empty()) {
    tricks_.push_back(Trick({&players_[0], &players_[1], &players_[2]}));
    return &players_[0];
  } else {
    auto const& lastTrick = tricks_.back();
    auto winner = lastTrick.eval();
    auto playerOrder = getOrder(winner, {&players_[0], &players_[1], &players_[2]});
    if (playerOrder[0] == single_)
      singleCount_ += lastTrick.count();
    else
      oppoCount_ += lastTrick.count();
    tricks_.push_back(Trick({playerOrder[0], playerOrder[1], playerOrder[2]}));
    return playerOrder[0];
  }
}

void Game::prevTrick() {
  _REQUIRE(!tricks_.empty(), "No previous trick.");
  Trick& lastTrick = tricks_.back();
  _REQUIRE(lastTrick.empty(), "Current trick not empty.");
  tricks_.pop_back();
  if (tricks_.empty()) return;
  lastTrick = tricks_.back();

  auto winner = lastTrick.eval();
  if (winner == single_)
    singleCount_ -= lastTrick.count();
  else
    oppoCount_ -= lastTrick.count();
}

uint64_t Game::getState() const {
  int curr = currentTrick().currentPlayer() == &players_[0]   ? 0
             : currentTrick().currentPlayer() == &players_[1] ? 1
                                                              : 2;
  uint64_t code = 0;
  for (auto const& p : players_) {
    code |= p.handCode();
  }
  code |= (uint64_t(curr) << 40);
  return code;
}

void Game::setSkat(const Hand& h) {
  for (auto const& c : skat_) singleCount_ -= eyes(c);
  skat_ = h;
  for (auto const& c : skat_) singleCount_ += eyes(c);
}

Player* Game::advance(Card const& c) {
  auto p = currentTrick().advance(c);
  if (currentTrick().done()) {
    return nextTrick();
  }
  return p;
}

void Game::regress() {
  if (currentTrick().empty()) {
    prevTrick();
  }
  currentTrick().regress();
}

HandRange Game::validCards() const { return currentTrick().validCards(); }

bool Game::currentIsSingle() const { return currentTrick().currentPlayer() == single_; }

bool Game::currentIsLead() const { return currentTrick().empty(); }

int Game::lastEyes() const { return tricks_[tricks_.size() - 2].count(); }

void Game::setSinglePlayer(const std::string& name) {
  auto it = std::find_if(players_.begin(), players_.end(), [&name](auto const& p) { return p.name() == name; });
  _REQUIRE(it != players_.end(), "Player not found with name " << name);
  std::size_t idx = std::distance(players_.begin(), it);
  single_ = &players_[idx];
}
}  // namespace skatomat