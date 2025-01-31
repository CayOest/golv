#pragma once

#include "types.h"
#include <random>

namespace skatomat
{
	Deck makeDeck();
	std::mt19937 shuffle(Deck& deck, unsigned seed = std::random_device()());
	void shuffle(Deck& deck, std::mt19937 gen);
	void sort(Hand& h);
	int eyes(Card c);
	int eyes(Hand const& h);
	bool isTrump(const Card& c);
}