#include <algorithm>
#include <array>
#include <cassert>
#include <golv/games/skat.hpp>
#include <golv/util/exception.hpp>
#include <golv/util/logging.hpp>
#include <iostream>

namespace golv {

bool less_suit(suit left, suit right) {
  if (left == right) return false;
  if (left == suit::diamonds) return true;
  if (right == suit::diamonds) return false;
  if (left == suit::hearts) return true;
  if (right == suit::hearts) return false;
  if (left == suit::spades) return true;
  return false;
}

bool less_kind(kind left, kind right) {
  if (left == right) return false;
  if (left == kind::ten) {
    if (right == kind::ace)
      return true;
    else
      return false;
  }
  if (right == kind::ten) {
    if (left == kind::ace)
      return false;
    else
      return true;
  }
  return static_cast<int>(left) > static_cast<int>(right);
}

// todo: implement ordering for tens
bool skat_card_order::operator()(card const& left, card const& right) const {
  if (left.kind_ == kind::jack) {
    if (right.kind_ == kind::jack) {
      return less_suit(left.suit_, right.suit_);
    } else {
      return false;
    }
  }
  if (right.kind_ == kind::jack) {
    return true;
  }
  if (left.suit_ == lead_suit) {
    if (right.suit_ == lead_suit) {
      return less_kind(left.kind_, right.kind_);
    } else {
      return false;
    }
  } else {
    if (right.suit_ == lead_suit) {
      return true;
    } else {
      if (left.suit_ == right.suit_) {
        return less_kind(left.kind_, right.kind_);
      } else {
        return less_suit(left.suit_, right.suit_);
      }
    }
  }
}

skat::move_range skat::legal_actions() const {
  skat::move_range legal;
  auto const& cards = state_[*current_player_];
  if (tricks_.empty() || tricks_.back().cards_.empty()) {
    legal = cards;
  } else {
    auto cmp = [](card const& left, card const& right) { return less_suit(left.suit_, right.suit_); };
    auto rng = std::equal_range(cards.begin(), cards.end(), tricks_.back().cards_.front(),
                                // todo: fix this for jacks
                                cmp);

    if (rng.first != rng.second) {
      legal = move_range(rng.first, rng.second);
    } else {
      legal = cards;
    }
  }
  GOLV_LOG_TRACE("legal_actions for player " << *current_player_ << ": " << legal);
  return legal;
}

skat::player_type skat::get_trick_winner() const {
  assert(!tricks_.empty());
  auto last_trick = tricks_.back().cards_;
  std::sort(std::begin(last_trick), std::end(last_trick), skat_card_order{last_trick.front().suit_});
  GOLV_LOG_TRACE("Sorted trick: " << last_trick);
  auto it = std::find(std::begin(tricks_.back().cards_), std::end(tricks_.back().cards_), last_trick.back());
  assert(it != tricks_.back().cards_.end());
  skat::player_type winner = (tricks_.back().leader_ + (it - tricks_.back().cards_.begin())) % num_players;
  GOLV_LOG_TRACE("Trick Winner = " << winner << " for trick " << tricks_.back().cards_);
  return winner;
}

skat::state_type skat::state() const {
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

void skat::apply_action(skat::move_type const& move) {
  GOLV_LOG_TRACE("apply_action for player " << *current_player_ << ": " << move);
  auto& cards = state_[*current_player_];
  auto it = std::find(std::begin(cards), std::end(cards), move);
  if (it == std::end(cards)) {
    throw std::domain_error("Card not in hand");
  }
  cards.erase(it);

  if (tricks_.empty()) tricks_.push_back({{}, *current_player_});

  tricks_.back().cards_.push_back(move);
  ++current_player_;

  if (tricks_.back().cards_.size() == num_players) {
    current_player_ = get_trick_winner();
    tricks_.push_back({{}, *current_player_});
  }
}

void skat::undo_action(skat::move_type const& move) {
  if (tricks_.empty()) {
    throw golv::exception("No tricks to undo");
  }
  if (!tricks_.back().cards_.empty()) {
    if (tricks_.back().cards_.back() != move) {
      throw std::domain_error("Cannot undo action");
    }
    --current_player_;
    GOLV_LOG_TRACE("undo_action for player " << *current_player_ << ": " << move);
    auto& cards = state_[*current_player_];
    cards.push_back(move);
    std::sort(cards.begin(), cards.end(), skat_card_order{});
    tricks_.back().cards_.pop_back();
  } else {
    if ((tricks_.end() - 2)->cards_.back() != move) {
      throw std::domain_error("Cannot undo action");
    }
    tricks_.pop_back();
    tricks_.back().cards_.pop_back();
    current_player_ = tricks_.back().leader_;
    --current_player_;
    GOLV_LOG_TRACE("undo_action for player " << *current_player_ << ": " << move);
    state_[*current_player_].push_back(move);
    std::sort(state_[*current_player_].begin(), state_[*current_player_].end(), skat_card_order{});
  }
}

skat::value_type count_eyes(skat::trick const& trick) {
  skat::value_type eyes = 0;
  for (auto const& card : trick.cards_) {
    switch (card.kind_) {
      case kind::jack:
        eyes += 2;
        break;
      case kind::ace:
        eyes += 11;
        break;
      case kind::ten:
        eyes += 10;
        break;
      case kind::king:
        eyes += 4;
        break;
      case kind::queen:
        eyes += 4;
    }
  }
  return eyes;
}

skat::value_type skat::value() const {
  if (!tricks_.empty() && tricks_.back().cards_.empty()) {
    return tricks_.back().leader_ == soloist_ ? count_eyes(tricks_[tricks_.size() - 2]) : 0;
  }

  return 0;
}

bool skat::is_terminal() const {
  for (size_t i = 0; i < skat::num_players; ++i) {
    if (!state_[i].empty()) return false;
  }
  return true;
}

bool skat::is_max() const { return *current_player_ == soloist_; }

bool skat::is_new_trick() const { return !tricks_.empty() && tricks_.back().cards_.empty(); }

void skat::deal(internal_state_type const& state) { state_ = state; }

const std::vector<skat::trick>& skat::tricks() const { return tricks_; }

skat::player_type skat::current_player() const { return *current_player_; }

void skat::set_soloist(player_type soloist) { soloist_ = soloist; }
}  // namespace golv