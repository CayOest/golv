#include <golv/traits/game.hpp>
#include <golv/traits/transposition_table.hpp>
#include <unordered_map>

namespace golv {

/**
 * mws_unordered_table is a simple TranspositionTable based on std::unordered_map.
 * It complies with the TranspositionTable concept.
 */
template <Game GameT>
struct mws_unordered_table {
  using storage_type = std::pair<typename GameT::value_type, typename GameT::value_type>;
  using map_type = std::unordered_map<typename GameT::state_type, storage_type>;

  map_type map_;

  constexpr bool is_memorable(GameT const& game) const {  //
    // todo! remove hash_me. work with std::hash instead
    return game.hash_me();
  }

  storage_type const& get(typename GameT::state_type const& state) const {
    const static storage_type _invalid = {std::numeric_limits<typename GameT::value_type>::lowest(),  //
                                          std::numeric_limits<typename GameT::value_type>::max()};
    auto it = map_.find(state);
    if (it != map_.end())
      return it->second;
    else
      return _invalid;
  }

  constexpr void set(typename GameT::state_type const& state, storage_type type_value) {  //
    map_[state] = type_value;
  }

  constexpr void update_lower(typename GameT::state_type const& state,  //
                              typename GameT::value_type const& value) {
    auto it = map_.find(state);
    if (it != map_.end()) {
      it->second.first = std::max(value, it->second.first);
    } else {
      map_[state] = {value, std::numeric_limits<typename GameT::value_type>::max()};
    }
  }

  constexpr void update_upper(typename GameT::state_type const& state,  //
                              typename GameT::value_type const& value) {
    auto it = map_.find(state);
    if (it != map_.end()) {
      it->second.second = std::min(value, it->second.second);
    } else {
      map_[state] = {std::numeric_limits<typename GameT::value_type>::lowest(), value};
    }
  }
};
}  // namespace golv