#pragma once

#include <golv/traits/game.hpp>
#include <type_traits>

namespace golv {

/**
 * TranspositionTable stores values for given game
 * states (which are the keys of the map).
 */
template <class T, class GameT>
concept TranspositionTable = requires(T t) {
  typename T::storage_type;

  // { t.is_memorable(GameT()) } -> std::convertible_to<bool>;
  { t.get(typename GameT::state_type()) } -> std::convertible_to<typename T::storage_type>;
  { t.set(typename GameT::state_type(), typename T::storage_type()) };
};

/**
 * no_table is a placeholder for no cached game states.
 * It complies with the TranspositionTable concept.
 *  TODO! Make no_table minimal and change TranspoTable to is_table && !no_table
 */
template <Game GameT>
struct no_table {
  using storage_type = int;

  constexpr bool is_memorable(GameT const&) const { return false; }

  storage_type const& get(typename GameT::state_type const&) const { return 0; }

  constexpr void set(typename GameT::state_type const&, storage_type) {
    // void
  }
};

template <class T>
struct with_table : public std::true_type {};

template <class GameT>
struct with_table<no_table<GameT>> : public std::false_type {};

}  // namespace golv