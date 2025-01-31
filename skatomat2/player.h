#pragma once

#include "singleton.h"
#include "rules.h"
#include "util.h"

#include <vector>

namespace skatomat
{
	class Player
	{
	public:
		explicit Player(const std::string& _name, const Hand& _hand);
		void remove(Card c);
		void add(Card c);
		Hand const& hand() const;
		uint64_t handCode() const;
		const std::string& name() const;

		virtual Card choose(const HandRange& valid) const;

	private:
		std::string name_;
		Hand hand_;
	};

}