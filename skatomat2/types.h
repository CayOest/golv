#pragma once

#include <array>
#include <cstdint>
#include <ostream>
#include <string>
#include <vector>

namespace skatomat {
enum class Suit { Diamonds = 9, Hearts = 10, Spades = 11, Clubs = 12 };

constexpr std::array<Suit, 4> allSuits = {Suit::Clubs, Suit::Spades, Suit::Hearts, Suit::Diamonds};

enum class Kind { Ace = 11, Ten = 10, King = 4, Queen = 3, Jack = 2, Nine = -1, Eight = -2, Seven = -3 };

constexpr std::array<Kind, 8> allKinds = {Kind::Ace,  Kind::Ten,  Kind::King,  Kind::Queen,
                                          Kind::Jack, Kind::Nine, Kind::Eight, Kind::Seven};

class Card {
 private:
  Kind kind_ = Kind::Seven;
  Suit suit_ = Suit::Diamonds;
  uint64_t code_ = 0;

 public:
  Card(const char* c);
  Card(Kind k, Suit s);
  Card() = default;
  Kind kind() const { return kind_; }
  Suit suit() const { return suit_; }
  uint64_t code() const { return code_; }
};

std::string toString(const Card& c);
bool operator==(const Card& left, const Card& right);
std::ostream& operator<<(std::ostream& os, const Card& c);

/*
 * Hand
 */
using Hand = std::vector<Card>;
using HandRange = std::pair<Hand::const_iterator, Hand::const_iterator>;
std::ostream& operator<<(std::ostream& os, const Hand& h);
std::string toString(const Hand& h);
Hand toHand(std::string s);

/*
 * Deck
 */
using Deck = std::array<Card, 32>;

/*
 * Trump
 */
enum class Trump { Grand = 24, Null = 23, Clubs = 12, Spades = 11, Hearts = 10, Diamonds = 9 };

enum class Position { Forehand = 0, Middlehand, Rearhand };

std::string to_string(Position pos);

struct Solution {
  int eyes = 0;
  Hand card = {"7d"};
};

struct Distribution {
  std::array<Hand, 4> hands;
  Trump trump;
  int declarer;
};
}  // namespace skatomat