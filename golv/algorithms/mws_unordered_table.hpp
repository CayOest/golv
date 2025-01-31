#include <bitset>
#include <golv/traits/game.hpp>
#include <golv/traits/transposition_table.hpp>
#include <golv/util/logging.hpp>
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

  storage_type const& get(typename GameT::state_type const& state) {
    const static storage_type _invalid = {std::numeric_limits<typename GameT::value_type>::lowest(),  //
                                          std::numeric_limits<typename GameT::value_type>::max()};
    auto it = map_.find(state);
    if (it == map_.end()) {
      it = map_.insert(it, {state, _invalid});
    }
    return it->second;
  }

  void set(typename GameT::state_type const& state, storage_type type_value) {  //
    map_[state] = type_value;
  }

  void update_lower(typename GameT::state_type const& state,  //
                    typename GameT::value_type const& value) {
    auto it = map_.find(state);
    if (it == map_.end()) {
      it = map_.insert(it, {state, {value, std::numeric_limits<typename GameT::value_type>::max()}});
    }
    it->second.first = std::max(value, it->second.first);
  }

  void update_upper(typename GameT::state_type const& state,  //
                    typename GameT::value_type const& value) {
    auto it = map_.find(state);
    if (it == map_.end()) {
      it = map_.insert(it, {state, {std::numeric_limits<typename GameT::value_type>::lowest(), value}});
    }
    it->second.second = std::min(value, it->second.second);
  }

  void report() const {
    GOLV_LOG_DEBUG("max_bucket_count = " << map_.max_bucket_count());
    GOLV_LOG_DEBUG("load_factor = " << map_.load_factor());
    GOLV_LOG_DEBUG("max_load_factor = " << map_.max_load_factor());
    GOLV_LOG_DEBUG("bucket_count = " << map_.bucket_count());
    GOLV_LOG_DEBUG("size = " << map_.size());
    int collisions = 0;
    for (size_t i = 0; i < map_.bucket_count(); ++i) {
      if (map_.bucket_size(i) > 1) collisions += (map_.bucket_size(i) - 1);
    }
    GOLV_LOG_DEBUG("collisions = " << collisions);
  }
};
}  // namespace golv