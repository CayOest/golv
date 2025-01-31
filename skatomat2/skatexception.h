#pragma once

#include <stdexcept>
#include <sstream>

namespace skatomat
{
	class SkatException : public std::runtime_error
	{
	public:
		SkatException(const std::string& what);
	};

#define _REQUIRE(condition,message) \
if (!(condition)) { \
    std::ostringstream _msg_stream; \
    _msg_stream << message; \
    throw SkatException(_msg_stream.str()); \
 } else 
}

#define _FAIL(message) \
{ \
    std::ostringstream _msg_stream; \
    _msg_stream << message; \
    throw SkatException(_msg_stream.str()); \
} 
