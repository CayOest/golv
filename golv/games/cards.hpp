#pragma once

#include <array>
#include <bitset>
#include <ostream>
#include <vector>
#include <cassert>
#include <algorithm>

namespace golv {

enum class suit
{
    spades,
    hearts,
    diamonds,
    clubs
};

enum class kind
{
    ace,
    king,
    queen,
    jack,
    ten,
    nine,
    eight,
    seven,
    six,
    five,
    four,
    three,
    deuce
};

class card {
 public:
  using code_type = std::bitset<64>;

  card() = default;
  card(const char* c);
  card(kind k, suit s);
  kind get_kind() const { return kind_; }
  suit get_suit() const { return suit_; }
  code_type code() const { return code_; }

 private:
  kind kind_;
  suit suit_;
  code_type code_ = 0;
};

std::string to_string(const card& c);
bool operator==(const card& left, const card& right);
std::ostream& operator<<(std::ostream& os, const card& c);
bool operator<(const card& left, const card& right);

/**
 * hand
 */
using hand = std::vector<card>;
std::ostream& operator<<(std::ostream& os, const hand& h);
std::string to_string(const hand& h);

template <size_t cards_per_suit>
hand create_deck() {
  hand deck;
  for (size_t s = 0; s < 4; ++s) {
    for (size_t k = 0; k < cards_per_suit; ++k) {
      deck.push_back(card{static_cast<kind>(k), static_cast<suit>(s)});
    }
  }
  return deck;
}

hand to_hand(std::string str);

hand create_bridge_deck();
hand create_skat_deck();

}  // namespace golv