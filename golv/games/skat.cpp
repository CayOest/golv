#include <golv/games/skat.hpp>
#include <iterator>

namespace golv {
bool
skat::operator<(golv::card const& left, golv::card const& right)
{
    if (left.kind_ == kind::jack) {
        if (right.kind_ == kind::jack) {
            return static_cast<int>(left.suit_) < static_cast<int>(right.suit_);
        } else
            return true;
    }
    if (right.kind_ == kind::jack)
        return false;

    if (left.suit_ == right.suit_) {
        return static_cast<int>(left.kind_) < static_cast<int>(right.kind_);
    }
    return static_cast<int>(left.suit_) < static_cast<int>(right.suit_);
}

bridge::move_range
skat::legal_actions() const
{
    bridge::move_range legal;
    auto const& cards = state_[*current_player_];
    if (tricks_.empty() || tricks_.back().cards_.empty()) {
        legal = cards;
    } else {
        auto const& lead_card = tricks_.back().cards_.front();
        if (lead_card.kind_ != kind::jack) {
            auto rng = std::equal_range(cards.begin(), cards.end(), lead_card, [](card const& left, card const& right) {
                return static_cast<int>(left.suit_) < static_cast<int>(right.suit_);
            });

            if (rng.first != rng.second) {
                legal = move_range(rng.first, rng.second);
            } else {
                legal = cards;
            }
        } else {
            auto jack_it = std::find_if(cards.begin(), cards.end(), [](card const& c) { return c.kind != kind::jack; });
            if (jack_it != cards.begin()) {
                legal = move_range(cards.begin(), jack_it);
            } else
                legal = cards;
        }
    }
    GOLV_LOG_TRACE("legal_actions for player " << *current_player_ << ": " << legal);
    return legal;
}

bool
skat_order::operator()(card const& left, card const& right) const
{
    if (left.kind == kind::jack) {
        if (right.kind == kind::jack) {
            return static_cast<int>(left.suit_) < static_cast < (right.suit_);
        }
    }
    if (right.suit == kind::jack)
        return false;
    return golv::less_card{ lead_suit }(left, right);
}

bridge::value_type
skat::value()
{
    if (!tricks_.empty() && tricks_.back().cards_.empty()) {
        int soloist = soloist_ % 2;
        return (tricks_.back().leader_ % 2) == soloist ? 1 : 0;
    }

    return 0;
}

bool
bridge::is_max() const
{
    return *current_player_ == soloist_;
}

}