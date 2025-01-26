#pragma once

#include <algorithm>
#include <golv/traits/game.hpp>
#include <iostream>
#include <limits>
#include <type_traits>
#include <unordered_map>

namespace golv {

/**
 * lookup_value_type is the type of the stored value in the TranspositionTable.
 */
enum class lookup_value_type
{
    exact,
    lower_bound,
    upper_bound
};

/**
 * TranspositionTable stores values (upper/lower bound or exact) for given game
 * states (which are the keys of the map).
 */
template<class T, class GameT>
concept TranspositionTable = requires(T t) {
    typename T::storage_type;

    {
        t.is_memorable(GameT())
    } -> std::convertible_to<bool>;
    {
        t.get(typename GameT::state_type())
    } -> std::convertible_to<typename T::storage_type>;
    {
        t.set(typename GameT::state_type(), lookup_value_type::exact, typename GameT::value_type())
    };
};

/**
 * no_table is a placeholder for no cached game states.
 * It complies with the TranspositionTable concept.
 *  TODO! Make no_table minimal and change TranspoTable to is_table && !no_table
 */
template<Game GameT>
struct no_table
{
    using storage_type = std::pair<lookup_value_type, typename GameT::value_type>;

    constexpr bool is_memorable(GameT const&) const { return false; }

    storage_type const& get(typename GameT::state_type const&) const
    {
        const static storage_type _value = { lookup_value_type::exact,
                                             std::numeric_limits<typename GameT::value_type>::lowest() };
        return _value;
    }

    constexpr void set(typename GameT::state_type const&, lookup_value_type, typename GameT::value_type const&) {}
};

template<class T>
struct with_table : public std::true_type
{};

template<class GameT>
struct with_table<no_table<GameT>> : public std::false_type
{};

/**
 * unordered_table is a simple TranspositionTable based on std::unordered_map.
 * It complies with the TranspositionTable concept.
 */
template<Game GameT>
struct unordered_table
{
    using storage_type = std::pair<lookup_value_type, typename GameT::value_type>;
    using map_type = std::unordered_map<typename GameT::state_type, storage_type>;

    map_type map_;

    constexpr bool is_memorable(GameT const& game) const { return game.is_max(); }

    storage_type const& get(typename GameT::state_type const& state) const
    {
        const static storage_type _invalid = { lookup_value_type::lower_bound,
                                               std::numeric_limits<typename GameT::value_type>::lowest() };
        auto it = map_.find(state);
        if (it != map_.end())
            return it->second;
        else
            return _invalid;
    }

    constexpr void set(typename GameT::state_type const& state,
                       lookup_value_type type,
                       typename GameT::value_type const& value)
    {
        map_[state] = { type, value };
    }
};

template <class GameT>
std::ostream& operator<<(std::ostream& os, unordered_table<GameT> const& t) {
  os << "Unordered Map = " << t.map_.size() << std::endl;
  using storage_type = typename unordered_table<GameT>::storage_type;
  using value_type = std::pair<typename GameT::state_type, storage_type>;
  std::vector<value_type> vec;
  for (auto const& [key, value] : t.map_) {
    vec.push_back(std::make_pair(key, value));
  }
  std::sort(vec.begin(), vec.end(), [](auto const& a, auto const& b) {
    if (a.first.size() != b.first.size()) return a.first.size() > b.first.size();
    return a.first < b.first;
  });
  for (auto const& [key, value] : vec) {
    os << key << " = " << static_cast<int>(value.first) << ", " << value.second << std::endl;
  }
  return os;
}

template<class GameT, TranspositionTable<GameT> TableT>
auto
lookup_before(TableT& table, GameT const& game, typename GameT::value_type& a, typename GameT::value_type& b)
  -> std::pair<bool, typename GameT::value_type>
{
    if (table.is_memorable(game)) {
        auto const& lookup = table.get(game.state());
        switch (lookup.first) {
            case lookup_value_type::exact:
                return { true, lookup.second };
            case lookup_value_type::lower_bound:
                a = std::max(a, lookup.second);
                break;
            case lookup_value_type::upper_bound:
                b = std::min(b, lookup.second);
                break;
        }
        if (b <= a) {
            if (game.is_max())
                return { true, a };
            else
                return { true, b };
        }
    }
    return { false, a };
}

} // namespace golv