#include <algorithm>
#include <array>
#include <cassert>
#include <golv/games/skat.hpp>
#include <golv/util/exception.hpp>
#include <golv/util/logging.hpp>
#include <iostream>

namespace golv {

namespace {
bool less_suit(suit left, suit right)
{
  if (left == right) return false;
  if (left == suit::diamonds) return true;
  if (right == suit::diamonds) return false;
  if (left == suit::hearts) return true;
  if (right == suit::hearts) return false;
  if (left == suit::spades) return true;
  return false;
}

bool less_kind(kind left, kind right)
{
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

bool is_trump(card const& c, trump t)
{
  if (c.get_kind() == kind::jack) return true;
  if (t == trump::grand) {
    return false;
  }
  return (static_cast<int>(t) == static_cast<int>(c.get_suit()));
}

bool less_trump(card const& left, card const& right)
{
  if (left.get_kind() == kind::jack) {
    if (right.get_kind() == kind::jack) {
      return less_suit(left.get_suit(), right.get_suit());
    }
    return false;
  }
  if (right.get_kind() == kind::jack) {
    return true;
  }
  return less_kind(left.get_kind(), right.get_kind());
}
}  // namespace

bool skat_card_order::operator()(card const& left, card const& right) const
{
  if (is_trump(left, trump_)) {
    if (is_trump(right, trump_)) {
      return less_trump(left, right);
    } else {
      return false;
    }
  }
  if (is_trump(right, trump_)) {
    return true;
  }
  if (left.get_suit() == lead_suit) {
    if (right.get_suit() == lead_suit) {
      return less_kind(left.get_kind(), right.get_kind());
    } else {
      return false;
    }
  } else {
    if (right.get_suit() == lead_suit) {
      return true;
    } else {
      if (left.get_suit() == right.get_suit()) {
        return less_kind(left.get_kind(), right.get_kind());
      } else {
        return less_suit(left.get_suit(), right.get_suit());
      }
    }
  }
}

skat::value_type count_eyes(hand const& cards) {
  skat::value_type eyes = 0;
  for (auto const& card : cards) {
    switch (card.get_kind()) {
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
        eyes += 3;
        break;
      default:
        break;
    }
  }
  return eyes;
}

skat::move_range skat::legal_actions() const {
  // pushing case
  if (soloist_ >= skat::num_players) {
    throw golv::exception("Soloist not set.");
  }
  if (state_[3].size() <= 1) {
    return state_[soloist_];
  }
  skat::move_range legal;
  auto const& cards = state_[*current_player_];
  if (tricks_.empty() || tricks_.back().cards_.empty()) {
    legal = cards;
  } else {
    if (tricks_.back().cards_.front().get_kind() == kind::jack) {
      auto it = std::find_if(cards.begin(), cards.end(), [](const card& c) { return c.get_kind() == kind::jack; });
      if (it != cards.end()) {
        legal = skat::move_range{it, cards.end()};
      } else {
        legal = cards;
      }
    } else {
      auto lead_suit = tricks_.back().cards_.front().get_suit();
      auto it = std::find_if(cards.begin(), cards.end(), [lead_suit](const card& c) {
        return c.get_kind() != kind::jack && c.get_suit() == lead_suit;
      });
      if (it != cards.end()) {
        auto it2 = std::find_if(it, cards.end(), [lead_suit](const card& c) {
          return c.get_kind() == kind::jack || c.get_suit() != lead_suit;
        });
        legal = skat::move_range{it, it2};
      } else {
        legal = cards;
      }
    }
  }
  GOLV_LOG_TRACE("legal_actions for player " << *current_player_ << ": " << legal);
  return legal;
}

skat::player_type skat::get_trick_winner() const {
  assert(!tricks_.empty());
  auto last_trick = tricks_.back().cards_;
  std::sort(std::begin(last_trick), std::end(last_trick), skat_card_order{last_trick.front().get_suit()});
  GOLV_LOG_TRACE("Sorted trick: " << last_trick);
  auto it = std::find(std::begin(tricks_.back().cards_), std::end(tricks_.back().cards_), last_trick.back());
  assert(it != tricks_.back().cards_.end());
  skat::player_type winner = (tricks_.back().leader_ + (it - tricks_.back().cards_.begin())) % num_players;
  GOLV_LOG_TRACE("Trick Winner = " << winner << " for trick " << tricks_.back().cards_);
  return winner;
}

skat::state_type skat::state() const {
  skat::state_type bits = 0;
  for (size_t i = 0; i < state_.size() - 1; ++i) {
    for (auto const& c : state_[i]) {
      bits |= c.code();
    }
  }
  if (*current_player_ == 1) {
    bits |= state_[3][0].code();
  } else if (*current_player_ == 2) {
    bits |= state_[3][1].code();
  }
  return bits;
}

bool skat::is_new_trick() const
{
  return !tricks_.empty() && tricks_.back().cards_.empty();
}

void skat::push(skat::move_type const& move)
{
  GOLV_LOG_TRACE("pushing " << move);
  state_[3].push_back(move);
  if (state_[3].size() == 2) {
    // done pushing
    current_player_ = 0;
    value_ = count_eyes(state_[3]);
  }
  return;
}

void skat::apply_action(skat::move_type const& move)
{
  GOLV_LOG_TRACE("apply_action for player " << *current_player_ << ": "
                                            << move);
  auto& cards = state_[*current_player_];
  auto it = std::find(std::begin(cards), std::end(cards), move);
  if (it == std::end(cards)) {
    throw golv::exception("Card not in hand");
  }
  cards.erase(it);

  // pushing phase
  if (state_[3].size() <= 1) {
    return push(move);
  }

  // playing phase
  if (tricks_.empty()) tricks_.push_back({{}, *current_player_});

  tricks_.back().cards_.push_back(move);
  ++current_player_;

  if (tricks_.back().cards_.size() == num_players) {
    current_player_ = get_trick_winner();
    auto eyes = count_eyes(tricks_.back().cards_);
    if (*current_player_ == soloist_) {
      value_ += eyes;
    } else {
      opp_value_ += eyes;
    }
    tricks_.back().eyes_ = eyes;
    tricks_.push_back({{}, *current_player_, 0});
  }
}

void skat::undo_action(skat::move_type const& move) {
  GOLV_LOG_TRACE("undo_action for player " << *current_player_ << ": " << move);
  if (tricks_.empty() ||
      (tricks_.size() == 1 && tricks_.back().cards_.empty())) {
    if (state_[3].empty()) {
      throw golv::exception("No move to undo");
    } else {
      // unpush
      GOLV_LOG_TRACE("unpushing " << move);
      auto& skat = state_[3];
      auto it = std::find(skat.begin(), skat.end(), move);
      if (it == skat.end()) {
        throw golv::exception("move not found in skat");
      }
      skat.erase(it);
      state_[soloist_].push_back(move);
      current_player_ = soloist_;
      value_ = 0;
      return;
    }
  }
  if (!tricks_.back().cards_.empty()) {
    if (tricks_.back().cards_.back() != move) {
      throw golv::exception("Cannot undo action");
    }
    --current_player_;
    auto& cards = state_[*current_player_];
    cards.push_back(move);
    std::sort(cards.begin(), cards.end(), skat_card_order{});
    tricks_.back().cards_.pop_back();
  } else {
    if ((tricks_.end() - 2)->cards_.back() != move) {
      throw golv::exception("Cannot undo action");
    }
    if (tricks_.back().leader_ == soloist_) {
      value_ -= (tricks_.end() - 2)->eyes_;
    } else {
      opp_value_ -= (tricks_.end() - 2)->eyes_;
    }
    (tricks_.end() - 2)->eyes_ = 0;
    tricks_.pop_back();
    tricks_.back().cards_.pop_back();
    current_player_ = tricks_.back().leader_;
    --current_player_;
    state_[*current_player_].push_back(move);
    std::sort(state_[*current_player_].begin(), state_[*current_player_].end(), skat_card_order{});
  }
}

skat::value_type skat::value() const
{
  return value_;
}

skat::value_type skat::opp_value() const
{
  return opp_value_;
}

bool skat::is_terminal() const {
  for (size_t i = 0; i < skat::num_players; ++i) {
    if (!state_[i].empty()) return false;
  }
  return true;
}

bool skat::is_max() const
{
  return *current_player_ == soloist_;
}

void skat::deal(golv::hand const& deck)
{
  if (deck.size() != 32) {
    throw golv::exception("Wrong number of cards: " +
                          std::to_string(deck.size()));
  }
  auto hand1 = golv::hand{deck.begin(), deck.begin() + 10};
  auto hand2 = golv::hand{deck.begin() + 10, deck.begin() + 20};
  auto hand3 = golv::hand{deck.begin() + 20, deck.begin() + 30};
  auto skat = golv::hand{deck[30], deck[31]};
  deal(hand1, hand2, hand3, skat);
}

void skat::deal(golv::hand const& first_hand, golv::hand const& second_hand,
                golv::hand const& third_hand, golv::hand const& skat)
{
  state_[0] = first_hand;
  state_[1] = second_hand;
  state_[2] = third_hand;
  state_[3] = skat;
  for (int i = 0; i < 4; ++i) {
    std::sort(state_[i].begin(), state_[i].end(), skat_card_order{});
  }
}

const std::vector<skat::trick>& skat::tricks() const
{
  return tricks_;
}

skat::player_type skat::current_player() const
{
  return *current_player_;
}

void skat::set_soloist(player_type soloist)
{
  soloist_ = soloist;
  std::copy(state_[3].begin(), state_[3].end(),
            std::back_inserter(state_[soloist_]));
  state_[3].clear();
  current_player_ = soloist_;
}

void skat::declare(trump t)
{
  trump_ = t;
  order_.trump_ = trump_;
  for (auto& h : state_) {
    std::sort(h.begin(), h.end(), order_);
  }
}

void skat::skip_pushing()
{
  auto legal = legal_actions();
  if (legal.size() < 3) throw golv::exception("Cannot skip pushing.");
  apply_action(legal.back());
  apply_action(*(legal.end() - 2));
}

golv::hand skat::blinds() const
{
  return state_[3];
}

std::ostream& operator<<(std::ostream& os, skat const& game) {
  for (auto const& cards : game.state_) {
    for (auto const& c : cards) {
      os << to_string(c) << " ";
    }
    os << " | ";
  }
  os << *game.current_player_;
  return os;
}

}  // namespace golv