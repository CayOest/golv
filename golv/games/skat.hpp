#pragma once

#include <golv/games/cards.hpp>
#include <golv/util/cyclic_number.hpp>
#include <array>
#include <string>

namespace golv {

struct skat_card_order {
  suit lead_suit = suit::clubs;

  bool operator()(card const& left, card const& right) const;
};

/**
 *   skat describes a simple skat game with no trump.
 *   the cards are dealt from a given deck which can be smaller than 32 cards.
 *   the number of players is 3.
 */
class skat {
 public:
  constexpr static size_t num_players = 3;

  using move_type = card;
  using move_range = std::vector<move_type>;
  using value_type = short;
  using player_type = unsigned short;
  using internal_state_type = std::array<move_range, num_players + 1>;
  using state_type = card::code_type;
  using cyclic_player_type = cyclic_number<player_type, num_players>;

  struct trick {
    std::vector<card> cards_;
    player_type leader_;
    value_type eyes_{0};
  };

 public:
  bool hash_me() const { return is_new_trick(); }
  move_range legal_actions() const;
  value_type value() const;
  value_type opp_value() const;
  value_type step_value() const;
  bool is_max() const;

  void set_soloist(player_type soloist);

  player_type current_player() const;
  void apply_action(move_type const& move);
  void undo_action(move_type const& move);

  bool is_terminal() const;
  state_type state() const;
  void deal(internal_state_type const& state);
  const std::vector<trick>& tricks() const;

  friend std::ostream& operator<<(std::ostream& os, skat const& game);

 private:
  value_type value_{0};
  value_type opp_value_{0};

  internal_state_type state_;
  cyclic_player_type current_player_ = 0;
  player_type soloist_ = 0;
  std::vector<trick> tricks_;

  player_type get_trick_winner() const;
  bool is_new_trick() const;
};

} // namespace golv
