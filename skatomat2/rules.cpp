#include "rules.h"

namespace skatomat
{
	bool GrandLess::Impl::operator()(const Card& first, const Card& second) const
	{
		if (first.kind() == second.kind()) 
			return first.suit() < second.suit();
		if (first.kind() == Kind::Jack)
			return false;
		if (second.kind() == Kind::Jack)
			return true;
		if (first.suit() == second.suit())
			return first.kind() < second.kind();
		return first.suit() < second.suit();
	}

	TrumpLess::TrumpLess(Suit t)
	{
		impl_ = std::make_shared<Impl>(t);
	}

	bool TrumpLess::Impl::operator()(const Card& first, const Card& second) const
	{
		if (first.kind() == Kind::Jack) {
			if (second.kind() == Kind::Jack)
				return static_cast<int>(first.suit()) < static_cast<int>(second.suit());
			else return false;
		}
		if (first.suit() == trump) {
			if (second.kind() == Kind::Jack)
				return true;
			if (second.suit() == trump) {
				return static_cast<int>(first.kind()) < static_cast<int>(second.kind());
			}
			else return false;
		}
		else {
			if (second.suit() == trump) return true;
			if (second.kind() == Kind::Jack) return true;
			if (first.suit() == second.suit()) 
				return static_cast<int>(first.kind()) < static_cast<int>(second.kind());
			return static_cast<int>(first.suit()) < static_cast<int>(second.suit());
		}	
	}

	NullLess::NullLess()
	{
		impl_ = std::make_shared<Impl>();
	}

	bool NullLess::Impl::operator()(const Card& first, const Card& second) const
	{
		if (first.suit() == second.suit())
			return first.kind() < second.kind();
		return first.suit() < second.suit();
	}

	bool HandOrder::operator()(const Hand& left, const Hand& right) const
	{
		if (left.empty()) return right.empty();
		if (right.empty()) return true;
		if (left.size() < right.size())
			return false;
		if (right.size() < left.size())
			return true;

		for (auto itLeft = left.begin(), itRight = right.begin();
			itLeft != left.end() && itRight != right.end();
			++itLeft, ++itRight) {
			if (*itLeft != *itRight) {
				return order(*itLeft, *itRight);
			}
		}

		return false;
	}
}