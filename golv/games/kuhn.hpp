#pragma once

#include <algorithm>
#include <golv/util/logging.hpp>
#include <random>
#include <stdexcept>
#include <string>
#include <vector>

namespace golv {

/**
 * Kuhn Poker implementation that satisfies the Game concept.
 */
class kuhn {
 public:
  using value_type = double;
  using player_type = int;  // 0 or 1
  using state_type = std::string;

  using move_type = char;          // 'c' for check, 'b' for bet, 'f' for fold
  using move_range = std::string;  // "cb", "cf", etc.

  using strategy_type = std::vector<double>;
  using information_set_type = std::string;

  kuhn() { reset(); }

  move_range legal_actions() const {
    if (state_.empty() || state_ == "x") return "xb";  // Player can check or bet
    if (state_ == "b") return "fc";                    // Player can call or fold
    if (state_ == "xb") return "fc";
    throw std::logic_error("No legal actions available in terminal state");
  }

  player_type current_player() const { return current_player_; }

  bool is_terminal() const {
    GOLV_LOG_TRACE("is_terminal: " << state_);
    return state_ == "xx" || state_ == "bf" || state_ == "bc" || state_ == "xbf" || state_ == "xbc";
  }

  value_type value() const {
    auto val = _value();
    if (current_player_ != max_player_) val *= -1;
    return val;
  }

  value_type _value() const {
    if (!is_terminal()) throw std::logic_error("Value requested for non-terminal state");

    auto opponent = (current_player_ + 1) % 2;
    // Payoff calculation
    if (state_ == "bf" || state_ == "xbf") {
      return 1;
    }
    if (state_ == "xx") return card_[current_player_] > card_[opponent] ? 1 : -1;  // Higher card wins

    if (state_ == "bc" || state_ == "xbc") {
      return card_[current_player_] > card_[opponent] ? 2 : -2;  // Higher card wins double the pot
    }
    throw std::logic_error("Unknown terminal state");
  }

  bool is_max() const { return current_player_ == max_player_; }

  state_type state() const { return std::to_string(card_[current_player_]) + "|" + state_; }

  void apply_action(move_type move) {
    if (legal_actions().find(move) == std::string::npos) {
      throw std::domain_error("Invalid move: " + std::string(1, move));
    }
    state_ += move;
    current_player_ = (current_player_ + 1) % 2;
  }

  void undo_action(move_type move) {
    if (state_.empty() || state_.back() != move) {
      throw std::logic_error("Cannot undo move: " + std::string(1, move));
    }
    state_.pop_back();
    current_player_ = (current_player_ + 1) % 2;
  }

  bool hash_me() const { return true; }

  void set_max(int player) { max_player_ = player; }

  void reset() {
    state_.clear();
    card_ = {0, 1};  // Default card assignment
    current_player_ = -1;
    max_player_ = 0;
  }

  bool is_chance_node() const { return current_player_ == -1; }

  void handle_chance_node() {
    // Randomly deal cards to players
    std::vector<int> deck = {0, 1, 2};
    std::shuffle(deck.begin(), deck.end(), std::mt19937{std::random_device{}()});
    deal(deck[0], deck[1]);
  }

  void deal(move_type card1, move_type card2) {
    card_ = {card1, card2};  // Default card assignment
    GOLV_LOG_TRACE("Dealt cards: " << card_[0] << ", " << card_[1]);
    current_player_ = 0;  // Set the game to start after the chance node
  }

 private:
  state_type state_;        // Current game state as a sequence of actions
  std::vector<int> card_;   // Cards assigned to the players
  int current_player_ = 0;  // Current player (0 or 1)
  int max_player_ = 0;      // Player to maximize value
};

}  // namespace golv
