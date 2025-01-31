#include "player.h"
#include "state.h"
#include "skatexception.h"

#include <algorithm>
#include <functional>

namespace skatomat
{
	Player::Player(const std::string& _name, const Hand& _hand)
		: name_(_name)
		, hand_(_hand)
	{
		sort(hand_);
	}

	void Player::remove(Card c)
	{
		_REQUIRE(!hand_.empty(), "Hand empty.");
		auto pr = std::equal_range(hand_.begin(), hand_.end(), c, GameState::instance().less());
		_REQUIRE(pr.first != pr.second, "" << c << " not found in hand " << hand_);
		hand_.erase(pr.first);
	}

	void Player::add(Card c)
	{
		if (hand_.empty()) hand_.push_back(c);
		else {
			auto it = std::lower_bound(hand_.begin(), hand_.end(), c, GameState::instance().less());
			hand_.insert(it, c);
		}
	}

	Hand const& Player::hand() const
	{
		return hand_;
	}

	uint64_t Player::handCode() const
	{
		uint64_t code = 0;
		for (auto const c : hand_)
		{
			code |= c.code();
		}
		return code;
	}

	const std::string& Player::name() const
	{
		return name_;
	}

	Card Player::choose(const HandRange& valid) const
	{
		_REQUIRE(valid.first < valid.second, "No valid cards.");
		return *valid.first;
	}
}
