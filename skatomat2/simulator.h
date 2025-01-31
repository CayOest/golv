#pragma once

#include "game.h"

namespace skatomat
{
	class Simulator
	{
	public:
		explicit Simulator(Game& g) : game_(g) {}
		virtual void run() = 0;

	protected:
		Game& game_;
	};
}
