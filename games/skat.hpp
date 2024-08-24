#pragma once

#include <golv/games/bridge.hpp>

namespace golv {

struct skat_order : public card_order
{
    bool operator()(card const& left, card const& right) const;
};

/**
 *   skat describes a simple skat game with jacks as trump (Grand).
 *   the cards are dealt from a given deck which can be smaller than 32 cards.
 *   the number of players is 3. this, however, can be easily changed in the future (as a template argument).
 *   the class suffices the Game concept.
 */
class skat : public golv::bridge
{
  private:
    bool operator<(golv::card const& left, golv::card const& right);

  public:
    constexpr static size_t num_players = 3;

  public:
    move_range legal_actions() const override;
    bool is_max() const override;
    value_type value() const override;

  private:
    void next_player();
};

} // namespace golv
