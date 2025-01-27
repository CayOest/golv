#pragma once

#include <exception>
#include <string>

namespace golv {

/**
 * base class for all golv::exceptions
 */
class exception : public std::exception
{
  private:
    std::string msg_;

  public:
    explicit exception(const char* msg)
      : msg_{ msg }
    {
    }

    explicit exception(const std::string& msg)
      : msg_{ msg }
    {
    }
};

} // namespace golv
