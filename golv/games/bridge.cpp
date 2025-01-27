#include <algorithm>
#include <array>
#include <cassert>
#include <golv/games/bridge.hpp>
#include <golv/util/exception.hpp>
#include <golv/util/logging.hpp>
#include <iostream>

namespace golv {

bool kind_less(kind left, kind right) {
  // > since ace = 0, king = 1, etc.
  return static_cast<int>(left) > static_cast<int>(right);
}

bool bridge_card_order::operator()(card const& left, card const& right) const {
  if (left.get_suit() == right.get_suit()) {
    return kind_less(left.get_kind(), right.get_kind());
  }
  if (left.get_suit() == lead_suit) {
    return false;
  }
  if (right.get_suit() == lead_suit) {
    return true;
  }
  return static_cast<int>(left.get_suit()) > static_cast<int>(right.get_suit());
}

bridge::move_range bridge::legal_actions() const {
  bridge::move_range legal;
  auto const& cards = state_[*current_player_];
  if (tricks_.empty() || tricks_.back().cards_.empty()) {
    legal = cards;
  } else {
    auto lead_card = tricks_.back().cards_.front();
    auto suit_order = [](card const& left, card const& right) { return left.get_suit() > right.get_suit(); };
    auto rng = std::equal_range(cards.begin(), cards.end(), lead_card, suit_order);

    if (rng.first != rng.second) {
      legal = move_range(rng.first, rng.second);
    } else {
      legal = cards;
    }
  }
  GOLV_LOG_TRACE("legal_actions for player " << *current_player_ << ": " << legal);
  return legal;
}

bridge::player_type
bridge::get_trick_winner() const
{
    assert(!tricks_.empty());
    auto last_trick = tricks_.back().cards_;
    std::sort(std::begin(last_trick), std::end(last_trick), bridge_card_order{last_trick.front().get_suit()});
    GOLV_LOG_TRACE("Sorted trick: " << last_trick);
    auto it = std::find(std::begin(tricks_.back().cards_), std::end(tricks_.back().cards_), last_trick.back());
    assert(it != tricks_.back().cards_.end());
    bridge::player_type winner = (tricks_.back().leader_ + (it - tricks_.back().cards_.begin())) % 4;
    GOLV_LOG_TRACE("Trick Winner = " << winner << " for trick " << tricks_.back().cards_);
    return winner;
}

bridge::state_type
bridge::state() const
{
    std::stringstream ss;
    if (!tricks_.empty() && !tricks_.back().cards_.empty()) {
      for (auto const& card : tricks_.back().cards_) {
        ss << card;
      }
      ss << " --- ";
    }
    for (auto const& cards : state_) {
        for (auto const& card : cards) {
            ss << card;
        }
        ss << " | ";
    }
    ss << *current_player_;
    return ss.str();
}

void
bridge::apply_action(bridge::move_type const& move)
{
    GOLV_LOG_TRACE("apply_action for player " << *current_player_ << ": " << move);
    auto& cards = state_[*current_player_];
    auto it = std::find(std::begin(cards), std::end(cards), move);
    assert(it != std::end(cards));
    cards.erase(it);

    if (tricks_.empty())
        tricks_.push_back({ {}, *current_player_ });

    tricks_.back().cards_.push_back(move);
    ++current_player_;

    if (tricks_.back().cards_.size() == 4) {
        current_player_ = get_trick_winner();
        tricks_.push_back({ {}, *current_player_ });
    }
}

void
bridge::undo_action(bridge::move_type const& move)
{
    assert(!tricks_.empty());
    if (!tricks_.back().cards_.empty()) {
        --current_player_;
        GOLV_LOG_TRACE("undo_action for player " << *current_player_ << ": " << move);
        auto& cards = state_[*current_player_];
        cards.push_back(move);
        std::sort(cards.begin(), cards.end(), bridge_card_order{});
        tricks_.back().cards_.pop_back();
    } else {
        tricks_.pop_back();
        tricks_.back().cards_.pop_back();
        current_player_ = tricks_.back().leader_;
        --current_player_;
        GOLV_LOG_TRACE("undo_action for player " << *current_player_ << ": " << move);
        state_[*current_player_].push_back(move);
        std::sort(state_[*current_player_].begin(), state_[*current_player_].end(), bridge_card_order{});
    }
}

bridge::value_type
bridge::value() const
{
    if (!tricks_.empty() && tricks_.back().cards_.empty()) {
        int soloist = soloist_ % 2;
        return (tricks_.back().leader_ % 2) == soloist ? 1 : 0;
    }

    return 0;
}

bool
bridge::is_terminal() const
{
    for (auto const& cards : state_) {
        if (!cards.empty())
            return false;
    }
    return true;
}

bool
bridge::is_max() const
{
    return *current_player_ % 2 == soloist_ % 2;
}

bool
bridge::is_new_trick() const
{
    return !tricks_.empty() && tricks_.back().cards_.empty();
}

void
bridge::deal(internal_state_type const& state)
{
    state_ = state;
}

const std::vector<bridge::trick>&
bridge::tricks() const
{
    return tricks_;
}

bridge::player_type
bridge::current_player() const
{
    return *current_player_;
}

void
bridge::set_soloist(player_type soloist)
{
    soloist_ = soloist;
}
} // namespace golv