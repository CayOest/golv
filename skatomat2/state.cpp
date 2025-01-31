#include "state.h"

namespace skatomat
{
	void GameState::setTrump(Trump t)
	{
		trump_ = t;
		setComparator();
	}

	CardOrder GameState::less() const
	{
		return *less_;
	}

	void GameState::setComparator()
	{
		switch (trump_)
		{
		case Trump::Grand: less_.reset(new GrandLess);
			break;
		case Trump::Null: less_.reset(new NullLess);
			break;
		case Trump::Clubs: less_.reset(new TrumpLess(Suit::Clubs));
			break;
		case Trump::Spades: less_.reset(new TrumpLess(Suit::Spades));
			break;
		case Trump::Hearts: less_.reset(new TrumpLess(Suit::Hearts));
			break;
		case Trump::Diamonds: less_.reset(new TrumpLess(Suit::Diamonds));
			break;
		}
	}
}