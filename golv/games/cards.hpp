#pragma once

#include <array>
#include <cstdint>
#include <ostream>
#include <vector>

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
 private:
  kind kind_;
  suit suit_;
  uint64_t code_ = 0;

 public:
  card(const char* c);
  card(kind k, suit s);
  card() = default;
  kind get_kind() const { return kind_; }
  suit get_suit() const { return suit_; }
  uint64_t code() const { return code_; }
};

std::string to_string(const card& c);
bool operator==(const card& left, const card& right);
std::ostream& operator<<(std::ostream& os, const card& c);

/**
 * hand
 */
using hand = std::vector<card>;
using hand_range = std::pair<hand::const_iterator, hand::const_iterator>;
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

bool operator<(const card& left, const card& right);

hand create_bridge_deck();
hand create_skat_deck();

}  // namespace golv