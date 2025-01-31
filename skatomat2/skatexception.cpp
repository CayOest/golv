#include "skatexception.h"

namespace skatomat
{
	SkatException::SkatException(const std::string& what)
		: std::runtime_error(what)
	{}
}