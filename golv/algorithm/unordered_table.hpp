#include <golv/traits/game.hpp>
#include <golv/traits/transposition_table.hpp>
#include <unordered_map>

namespace golv {
/**
 * lookup_value_type is the type of the stored value in the TranspositionTable.
 */
enum class lookup_value_type { exact, lower_bound, upper_bound };

/**
 * unordered_table is a simple TranspositionTable based on std::unordered_map.
 * It complies with the TranspositionTable concept.
 */
template <Game GameT>
struct unordered_table {
  using storage_type = std::pair<lookup_value_type, typename GameT::value_type>;
  using map_type = std::unordered_map<typename GameT::state_type, storage_type>;

  map_type map_;

  auto size() const { return map_.size(); }

  constexpr bool is_memorable(GameT const& game) const {  //
    return game.hash_me();
    // return game.is_max();
  }

  storage_type const& get(typename GameT::state_type const& state) const {
    const static storage_type _invalid = {lookup_value_type::lower_bound,
                                          std::numeric_limits<typename GameT::value_type>::lowest()};
    auto it = map_.find(state);
    if (it != map_.end())
      return it->second;
    else
      return _invalid;
  }

  constexpr void set(typename GameT::state_type const& state, storage_type type_value) { map_[state] = type_value; }

  constexpr void set(typename GameT::state_type const& state, lookup_value_type type,
                     typename GameT::value_type const& value) {
    map_[state] = {type, value};
  }
};

// template <class state_type, class storage_type>
// struct _sorter {
//   void operator()(std::vector<std::pair<state_type, storage_type>>& vec) {
//     std::sort(vec.begin(), vec.end(), [](auto const& a, auto const& b) {
//       if (a.first.size() != b.first.size()) return a.first.size() > b.first.size();
//       return a.first < b.first;
//     });
//   }
// };

// template <class storage_type, size_t N>
// struct _sorter<std::bitset<N>, storage_type> {
//   void operator()(std::vector<std::pair<std::bitset<N>, storage_type>>& vec) {
//     // todo
//     // std::sort(vec.begin(), vec.end(),
//     //           [](auto const& a, auto const& b) { return a.first.to_ullong() < b.first.to_ullong(); });
//   }
// };

template <class GameT>
std::ostream& operator<<(std::ostream& os, unordered_table<GameT> const& t) {
  os << "Unordered Map = " << t.map_.size() << std::endl;
  using storage_type = typename unordered_table<GameT>::storage_type;
  using value_type = std::pair<typename GameT::state_type, storage_type>;
  std::vector<value_type> vec;
  for (auto const& [key, value] : t.map_) {
    vec.push_back(std::make_pair(key, value));
  }
  //_sorter<typename GameT::state_type, storage_type>{}(vec);
  for (auto const& [key, value] : vec) {
    os << key << " = " << static_cast<int>(value.first) << ", " << value.second << std::endl;
  }
  return os;
}

template <class GameT, TranspositionTable<GameT> TableT>
auto lookup_before(TableT& table, GameT const& game, typename GameT::value_type& a,
                   typename GameT::value_type& b) -> std::pair<bool, typename GameT::value_type> {
  using table_type = TableT;
  if constexpr (with_table<table_type>::value) {
    if (table.is_memorable(game)) {
      auto const& lookup = table.get(game.state());
      switch (lookup.first) {
        case lookup_value_type::exact:
          return {true, lookup.second};
        case lookup_value_type::lower_bound:
          a = std::max(a, lookup.second);
          break;
        case lookup_value_type::upper_bound:
          b = std::min(b, lookup.second);
          break;
      }
      if (b <= a) {
        if (game.is_max())
          return {true, a};
        else
          return {true, b};
      }
    }
    return {false, a};
  } else {
    return {false, std::numeric_limits<typename GameT::value_type>::lowest()};
  }
}
}  // namespace golv