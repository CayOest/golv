#pragma once

#include "trick.h"

#include <vector>
#include <unordered_map>

namespace skatomat
{
	class Game
	{
	public:
		explicit Game(Player const& fh, Player const& mh, Player const& bh);
		Game(const Game& g);
		
		std::pair<int, int> counts() const;
		size_t trickCount() const;
		uint64_t getState() const;
		void setSkat(const Hand& h);
		Hand const& skat() const { return skat_;  }
		
		Player* advance(Card const& c);
		void regress();
		HandRange validCards() const;
		bool currentIsSingle() const;
		bool currentIsLead() const;
		int lastEyes() const;
		void setSinglePlayer(const std::string& name);
		Player* singlePlayer() { return single_;  }

		std::array<Player const*, 3> players() const
		{
			std::array<Player const*, 3> res = { &players_[0], &players_[1], &players_[2] };
			return res;
		}

		std::array<Player*, 3> players() 
		{
			std::array<Player*, 3> res = { &players_[0], &players_[1], &players_[2] };
			return res;
		}

		Trick const& currentTrick() const;
	private:
		Player* nextTrick();
		void prevTrick();

		Trick& currentTrick();

		std::array<Player, 3> players_;
		Player* single_;

		std::vector<Trick> tricks_;
		int singleCount_ = 0;
		int oppoCount_ = 0;
		Hand skat_;
	};

}
