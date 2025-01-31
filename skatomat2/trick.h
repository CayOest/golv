#pragma once

#include "player.h"

namespace skatomat
{
	class Game;

	class Trick
	{
	public:
		Trick(const std::array<Player*, 3>& players);

		Player* advance(Card c);
		void regress();
		int count() const;
		Player* eval() const;
		HandRange validCards() const;
		bool empty() const;
		Player* currentPlayer() const;
		bool done() const;
		int size() const;
		Card const& operator[](size_t i) const { return cards[i]; }
		Trick clone(Game const& from, Game& to) const;

		void setCards(const std::array<Card, 3>& c) { cards = c;  }
		void setCurr(int i) { curr = i; }

	private:
		std::array<Player*, 3> players;
		std::array<Card, 3> cards;
		int curr = 0;
	};
}