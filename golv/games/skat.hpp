#pragma once

#include <golv/games/cards.hpp>
#include <golv/util/cyclic_number.hpp>
#include <array>
#include <string>

namespace golv {

enum class trump
{
  diamonds = static_cast<int>(suit::diamonds),
  hearts = static_cast<int>(suit::hearts),
  spades = static_cast<int>(suit::spades),
  clubs = static_cast<int>(suit::clubs),
  grand
};

struct skat_card_order {
  suit lead_suit = suit::clubs;
  trump trump_ = trump::grand;

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

  /**
   * Return a list of all legal actions for the current player.
   */
  move_range legal_actions() const;

  /**
   * Return the current value of the game, i. e. the cumulative eyes
   * of the soloist.
   */
  value_type value() const;

  /**
   * Return the cumulative eyes of the opposition.
   */
  value_type opp_value() const;

  /**
   * Check whether the current player is the maximizing player.
   */
  bool is_max() const;

  /**
   * This should be set AFTER deal().
   * It moves all skat cards to the soloist.
   */
  void set_soloist(player_type soloist);

  /**
   * Fast pushing.
   */
  void skip_pushing();

  /**
   * Declare a trump.
   */
  void declare(trump t);

  player_type current_player() const;
  void apply_action(move_type const& move);
  void undo_action(move_type const& move);

  /**
   * Check whether the game is in a terminal state, i. e. no more moves
   * possible.
   */
  bool is_terminal() const;
  state_type state() const;
  const std::vector<trick>& tricks() const;

  /**
   * Deal a skat deck of 32 cards such that player i gets (deck[i*10], ...,
   * deck[i*10+9]). The skat is (deck[30], deck[31]).
   */
  void deal(golv::hand const& deck);

  /**
   * Deal certain hands to the players which can be smaller than 10.
   */
  void deal(golv::hand const& first_hand, golv::hand const& second_hand,
            golv::hand const& third_hand, golv::hand const& skat);

  bool hash_me() const
  {
    return is_new_trick();
  }

  golv::hand blinds() const;

  friend std::ostream& operator<<(std::ostream& os, skat const& game);

 private:
  player_type get_trick_winner() const;
  bool is_new_trick() const;
  void push(skat::move_type const& move);

  value_type value_{0};
  value_type opp_value_{0};

  internal_state_type state_;
  std::vector<trick> tricks_;
  player_type soloist_ = 100;
  cyclic_player_type current_player_ = 0;
  trump trump_ = trump::grand;
  skat_card_order order_{suit::clubs, trump::grand};
};

} // namespace golv
