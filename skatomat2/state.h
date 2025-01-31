#pragma once

#include "singleton.h"
#include "rules.h"

#include <memory>
#include "player.h"

namespace skatomat
{
	enum class LogLevel
	{
		Trace,
		None
	};

	class GameState : public Singleton<GameState>
	{
		friend class Singleton<GameState>;
	public:
		void setTrump(Trump t);
		CardOrder less() const;

		Trump trump() const { return trump_; }
		void setLogLevel(LogLevel l) { level_ = l; }
		LogLevel logLevel() const { return level_;  }

	private:
		GameState() {}
		void setComparator();
		Trump trump_;
		std::unique_ptr<CardOrder> less_;
		LogLevel level_;
	};
}