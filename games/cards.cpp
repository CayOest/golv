
#include <golv/games/cards.hpp>
#include <golv/games/exception.hpp>
#include <iterator>

namespace golv {

bool
operator==(golv::card const& left, golv::card const& right)
{
    return left.suit_ == right.suit_ && left.kind_ == right.kind_;
}

bool
operator<(golv::card const& left, golv::card const& right)
{
    if (left.suit_ == right.suit_) {
        return static_cast<int>(left.kind_) < static_cast<int>(right.kind_);
    }
    return static_cast<int>(left.suit_) < static_cast<int>(right.suit_);
}

std::ostream&
operator<<(std::ostream& os, card const& c)
{
    switch (c.kind_) {
        case kind::ace:
            os << "A";
            break;
        case kind::king:
            os << "K";
            break;
        case kind::queen:
            os << "Q";
            break;
        case kind::jack:
            os << "J";
            break;
        case kind::ten:
            os << "T";
            break;
        case kind::nine:
            os << "9";
            break;
        case kind::eight:
            os << "8";
            break;
        case kind::seven:
            os << "7";
            break;
        case kind::six:
            os << "6";
            break;
        case kind::five:
            os << "5";
            break;
        case kind::four:
            os << "4";
            break;
        case kind::three:
            os << "3";
            break;
        case kind::deuce:
            os << "2";
            break;
    }
    switch (c.suit_) {
        case suit::spades:
            os << "s";
            break;
        case suit::hearts:
            os << "h";
            break;
        case suit::diamonds:
            os << "d";
            break;
        case suit::clubs:
            os << "c";
            break;
    }
    return os;
}

std::ostream&
operator<<(std::ostream& os, deck const& _deck)
{
    std::copy(_deck.begin(), _deck.end(), std::ostream_iterator<card>(os, ""));
    return os;
}

deck
create_deck(size_t cards_per_suit)
{
    if (cards_per_suit > 13) {
        throw golv::exception("Too many cards per suit: " + std::to_string(cards_per_suit));
    }
    deck _deck;
    _deck.reserve(cards_per_suit * 4);
    for (int _suit = 0; _suit < 4; ++_suit) {
        for (int _kind = 0; _kind < static_cast<int>(cards_per_suit); ++_kind) {
            _deck.push_back({ static_cast<golv::kind>(_kind), static_cast<golv::suit>(_suit) });
        }
    }
    return _deck;
}
}