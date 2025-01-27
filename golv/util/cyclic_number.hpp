#pragma once

#include <concepts>
#include <golv/util/exception.hpp>
#include <utility>

namespace golv {

template<class T>
concept UnsignedIntegral = !std::is_signed_v<T> && std::is_integral_v<T>;

template<UnsignedIntegral T, T UpperBound>
class cyclic_number
{
  public:
    explicit cyclic_number(T init = {})
      : counter_(init)
    {
    }

    cyclic_number& operator++()
    {
        ++counter_;
        if (counter_ == UpperBound) {
            counter_ = 0;
        }
        return *this;
    }

    cyclic_number& operator--()
    {
        --counter_;
        if (counter_ >= UpperBound) {
            counter_ = UpperBound - 1;
        }
        return *this;
    }

    cyclic_number operator++(int)
    {
        cyclic_number copy(*this);
        ++(*this);
        return copy;
    }

    cyclic_number operator--(int)
    {
        cyclic_number copy(*this);
        --(*this);
        return copy;
    }

    T operator*() const { return counter_; }

    cyclic_number(T const& number)
      : counter_(number)
    {
        if (number >= UpperBound) {
            throw golv::exception("not a valid assignment for cyclic_number");
        }
    }

    cyclic_number& operator=(T const& number)
    {
        if (number >= UpperBound) {
            throw golv::exception("not a valid assignment for cyclic_number");
        }
        counter_ = number;
        return *this;
    }

  private:
    T counter_{};
};

} // namespace golv