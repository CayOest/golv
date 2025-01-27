#include <golv/games/bridge.hpp>
#include <golv/games/skat.hpp>

golv::bridge create_game(size_t cards_per_suit = 13);
golv::bridge create_random_game(size_t cards_per_suit = 13, int rotate = 0, unsigned seed = 91189);
golv::skat create_random_skat_game(size_t cards_per_player = 10, int rotate = 0, unsigned seed = 91189);