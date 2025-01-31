#include "types.h"

#include <boost/algorithm/string.hpp>
#include <boost/bimap/bimap.hpp>

#include "skatexception.h"

namespace skatomat {
using namespace boost::bimaps;
namespace {
using KindMap = bimap<Kind, char>;
KindMap makeKindMap() {
  KindMap map;
  map.insert({Kind::Ace, 'A'});
  map.insert({Kind::Ten, 'T'});
  map.insert({Kind::King, 'K'});
  map.insert({Kind::Queen, 'Q'});
  map.insert({Kind::Jack, 'J'});
  map.insert({Kind::Nine, '9'});
  map.insert({Kind::Eight, '8'});
  map.insert({Kind::Seven, '7'});
  return map;
}

const KindMap& kindChars() {
  const static KindMap map = makeKindMap();
  return map;
};

using SuitMap = bimap<Suit, char>;

SuitMap makeSuitMap() {
  SuitMap map;
  map.insert({Suit::Clubs, 'c'});
  map.insert({Suit::Spades, 's'});
  map.insert({Suit::Hearts, 'h'});
  map.insert({Suit::Diamonds, 'd'});
  return map;
}

const SuitMap& suitChars() {
  const static SuitMap map = makeSuitMap();
  return map;
};

Card toCard(const std::string& s) {
  assert(s.size() == 2);
  auto it = kindChars().right.find(s.front());
  _REQUIRE(it != kindChars().right.end(), "Error parsing char " + s.front());
  auto it2 = suitChars().right.find(s.back());
  _REQUIRE(it2 != suitChars().right.end(), "Error parsing char " + s.back());
  return Card{it->second, it2->second};
}
}  // namespace

uint64_t kindShift(Kind k) {
  switch (k) {
    case Kind::Ace:
      return 0;
    case Kind::Ten:
      return 1;
    case Kind::King:
      return 2;
    case Kind::Queen:
      return 3;
    case Kind::Jack:
      return 4;
    case Kind::Nine:
      return 5;
    case Kind::Eight:
      return 6;
    case Kind::Seven:
      return 7;
  }
  return 0;
}

uint64_t suitShift(Suit s) {
  switch (s) {
    case Suit::Clubs:
      return 0;
    case Suit::Spades:
      return 8;
    case Suit::Hearts:
      return 16;
    case Suit::Diamonds:
      return 24;
  }
  return 0;
}

uint64_t toCode(Kind kind, Suit s) {
  uint64_t code = (uint64_t(1) << kindShift(kind));
  code <<= suitShift(s);
  return code;
}

Card::Card(const char* c) {
  Card x = toCard(std::string(c));
  this->kind_ = x.kind();
  this->suit_ = x.suit();
  code_ = toCode(this->kind(), this->suit());
}

Card::Card(Kind k, Suit s) : kind_(k), suit_(s), code_(toCode(k, s)) {}

std::string toString(const Card& c) {
  auto it = kindChars().left.find(c.kind());
  if (it == kindChars().left.end()) throw skatomat::SkatException("Error parsing kind");
  auto it2 = suitChars().left.find(c.suit());
  if (it2 == suitChars().left.end()) throw skatomat::SkatException("Error parsing suit");
  return {it->second, it2->second};
}

bool operator==(const Card& left, const Card& right) {
  return left.kind() == right.kind() && left.suit() == right.suit();
}

std::ostream& operator<<(std::ostream& os, const Card& c) {
  os << toString(c);
  return os;
}

std::ostream& operator<<(std::ostream& os, const Hand& h) {
  for (auto const c : h) os << c << " ";
  return os;
}

std::string toString(const Hand& h) {
  std::stringstream ss;
  ss << h;
  return ss.str();
}

Hand toHand(std::string s) {
  Hand h;
  std::vector<std::string> v;
  boost::trim(s);
  boost::split(v, s, boost::is_any_of(", "));
  std::transform(v.begin(), v.end(), std::back_inserter(h), [](const std::string& c) { return toCard(c); });
  return h;
}

std::string to_string(Position pos) {
  switch (pos) {
    case Position::Forehand:
      return "FH";
    case Position::Middlehand:
      return "MH";
    case Position::Rearhand:
      return "RH";
  }
  _FAIL("Position not found.")
}

}  // namespace skatomat