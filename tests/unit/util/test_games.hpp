#pragma once

#include <golv/games/bridge.hpp>
#include <golv/games/skat.hpp>

// game results from create_random_game(13)
golv::bridge default_game_13();

// game results from create_random_game(5, rotation)
golv::bridge default_game_5(int rotation = 0);

// game results from create_random_game(3)
golv::bridge default_game_3();

// game results from create_random_skat_game(10)
golv::skat default_skat_game_10(int rotation = 0, unsigned soloist = 0,
                                bool skip_pushing = true);
