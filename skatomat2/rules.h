#pragma once

#include "types.h"

#include <memory>

namespace skatomat
{
	class CardOrder
	{
	public:
		bool operator()(const Card& lhs, const Card& rhs) const
		{
			return impl_->operator()(lhs, rhs);
		}

	protected:
		class Impl
		{
		public:
			virtual bool operator()(const Card& lhs, const Card& rhs) const = 0;
		};

		std::shared_ptr<Impl> impl_;
	};

	class GrandLess : public CardOrder
	{
	private:
		class Impl : public CardOrder::Impl
		{
			bool operator()(const Card& lhs, const Card& rhs) const final;
		};
	public:
		GrandLess()
		{
			impl_ = std::make_shared<Impl>();
		}
	};

	struct TrumpLess : public CardOrder
	{
	private:
		class Impl : public CardOrder::Impl
		{
		private:
			Suit trump;
			bool operator()(const Card& lhs, const Card& rhs) const final;
		public:
			Impl(Suit t) : trump(t) {}
		};

	public:
		TrumpLess(Suit t);
	};

	struct NullLess : public CardOrder
	{
	private:
		class Impl : public CardOrder::Impl
		{
			bool operator()(const Card& lhs, const Card& rhs) const final;
		};
	public:
		NullLess();
	};

	struct HandOrder
	{
		CardOrder order;

		bool operator()(const Hand& left, const Hand& right) const;
	};
}