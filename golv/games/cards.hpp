#pragma once

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

struct card
{
    kind kind_;
    suit suit_;
};

bool
operator==(golv::card const& left, golv::card const& right);

bool
operator<(golv::card const& left, golv::card const& right);

using deck = std::vector<card>;

deck
create_deck(size_t cards_per_suit);

std::ostream&
operator<<(std::ostream& os, card const& c);

std::ostream&
operator<<(std::ostream& os, deck const& deck);
}