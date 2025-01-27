#pragma once

#include <golv/games/cards.hpp>
#include <golv/util/cyclic_number.hpp>

#include <array>
#include <string>

namespace golv {

struct bridge_card_order {
  suit lead_suit = suit::spades;

  bool operator()(card const& left, card const& right) const;
};

/**
 *   bridge describes a simple bridge game with no trump.
 *   the cards are dealt from a given deck which can be smaller than 52 cards.
 *   the number of players is 4. this, however, can be easily changed in the future (as a template argument).
 *   the class suffices the Game concept.
 */
class bridge
{
  public:
    constexpr static size_t num_players = 4;

    using move_type = card;
    using move_range = std::vector<move_type>;
    using value_type = short;
    using player_type = unsigned short;
    using internal_state_type = std::array<move_range, num_players>;
    using state_type = std::string;
    using cyclic_player_type = cyclic_number<player_type, num_players>;

    struct trick
    {
        std::vector<card> cards_;
        player_type leader_;
    };

  private:
    internal_state_type state_;
    cyclic_player_type current_player_ = 0;
    player_type soloist_ = 0;
    std::vector<trick> tricks_;

    player_type get_trick_winner() const;

  public:
   virtual move_range legal_actions() const;
   virtual value_type value() const;
   virtual bool is_max() const;

   void set_soloist(player_type soloist);

   player_type current_player() const;
   void apply_action(move_type const& move);
   void undo_action(move_type const& move);

   bool is_terminal() const;
   bool is_new_trick() const;
   state_type state() const;
   void deal(internal_state_type const& state);
   const std::vector<trick>& tricks() const;

  private:
    value_type value_{ 0 };

    void next_player();
};

} // namespace golv
