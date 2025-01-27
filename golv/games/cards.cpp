
#include <golv/games/cards.hpp>
#include <golv/util/exception.hpp>
#include <sstream>

namespace golv {

uint64_t to_code(kind kind, suit s) {
  uint64_t code = (uint64_t(1) << static_cast<uint64_t>(kind));
  code <<= (static_cast<uint64_t>(static_cast<uint64_t>(s) * (static_cast<uint64_t>(kind::deuce) + 1)));
  return code;
}

kind to_kind(char k) {
  switch (k) {
    case 'A':
      return kind::ace;
    case 'K':
      return kind::king;
    case 'Q':
      return kind::queen;
    case 'J':
      return kind::jack;
    case 'T':
      return kind::ten;
    case '9':
      return kind::nine;
    case '8':
      return kind::eight;
    case '7':
      return kind::seven;
    case '6':
      return kind::six;
    case '5':
      return kind::five;
    case '4':
      return kind::four;
    case '3':
      return kind::three;
    case '2':
      return kind::deuce;
  }
  throw golv::exception("Error parsing kind from char: " + k);
}

suit to_suit(char s) {
  switch (s) {
    case 's':
      return suit::spades;
    case 'h':
      return suit::hearts;
    case 'd':
      return suit::diamonds;
    case 'c':
      return suit::clubs;
  }
  throw golv::exception("Error parsing suit from char: " + s);
}

card to_card(std::string const& str) {
  if (str.size() != 2) throw golv::exception("Error parsing card");
  auto k = to_kind(str.front());
  auto s = to_suit(str.back());
  return card{k, s};
}

card::card(const char* c) {
  card x = to_card(std::string(c));
  this->kind_ = x.get_kind();
  this->suit_ = x.get_suit();
  code_ = to_code(this->get_kind(), this->get_suit());
}

card::card(kind k, suit s) : kind_(k), suit_(s), code_(to_code(k, s)) {}

char to_char(suit s) {
  switch (s) {
    case suit::spades:
      return 's';
    case suit::hearts:
      return 'h';
    case suit::diamonds:
      return 'd';
    case suit::clubs:
      return 'c';
  }
  throw golv::exception("Error parsing suit");
}

char to_char(kind k) {
  switch (k) {
    case kind::ace:
      return 'A';
    case kind::king:
      return 'K';
    case kind::queen:
      return 'Q';
    case kind::jack:
      return 'J';
    case kind::ten:
      return 'T';
    case kind::nine:
      return '9';
    case kind::eight:
      return '8';
    case kind::seven:
      return '7';
    case kind::six:
      return '6';
    case kind::five:
      return '5';
    case kind::four:
      return '4';
    case kind::three:
      return '3';
    case kind::deuce:
      return '2';
  }
  throw golv::exception("Error parsing kind");
}

std::string to_string(const card& c) {
  return std::string(1, to_char(c.get_kind())) + std::string(1, to_char(c.get_suit()));
}

bool operator==(const card& left, const card& right) {
  return left.get_kind() == right.get_kind() && left.get_suit() == right.get_suit();
}

std::ostream& operator<<(std::ostream& os, const card& c) {
  os << to_string(c);
  return os;
}

std::ostream& operator<<(std::ostream& os, const hand& h) {
  for (auto const c : h) os << c << " ";
  return os;
}

std::string to_string(const hand& h) {
  std::stringstream ss;
  ss << h;
  return ss.str();
}

bool operator<(const card& left, const card& right) { return left.code() < right.code(); }

hand create_bridge_deck() { return create_deck<13>(); }
hand create_skat_deck() { return create_deck<8>(); }

}  // namespace golv