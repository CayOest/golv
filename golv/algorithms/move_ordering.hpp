#pragma once

#include <type_traits>

namespace golv {

struct no_ordering
{};

template<class T>
struct with_ordering : public std::true_type
{};

template<>
struct with_ordering<no_ordering> : std::false_type
{};

} // namespace golv