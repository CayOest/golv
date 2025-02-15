#include "test_games.hpp"

#include <algorithm>
#include <iostream>

using namespace golv;

bridge default_game_13() {
  std::array<std::string, 4> card_strings = {
      //
      "Qc4d7d9dJdQd3h5h6h7hKh2sKs",  //
      "5c8c9cTcKc6d8d2h3s6sJsQsAs",  //
      "4c6cAcKd8h9hThQhAh7s8s9sTs",  //
      "2c3c7cJc2d3d5dTdAd4hJh4s5s"   //
  };
  std::array<hand, 4> cards;
  for (size_t i = 0; i < 4; ++i) {
    for (size_t j = 0; j < 13; ++j) {
      cards[i].push_back(card(card_strings[i].substr(j * 2, 2).c_str()));
    }
  }
  bridge game;
  game.deal(cards);
  return game;
}

// game results from create_random_game(5, rotation)
bridge default_game_5(int rotation) {
  std::array<std::string, 4> card_strings = {
      //
      "9dJd3h2sKs",  //
      "Qc7d6h7hKh",  //
      "8c4dQd5hQs",  //
      "5cKc6d8d3s"   //
  };
  std::array<hand, 4> cards;
  for (size_t i = 0; i < 4; ++i) {
    for (size_t j = 0; j < 5; ++j) {
      cards[i].push_back(card(card_strings[i].substr(j * 2, 2).c_str()));
    }
  }
  std::rotate(cards.begin(), cards.begin() + rotation, cards.end());

  bridge game;
  game.deal(cards);
  return game;
}

// game results from create_random_game(13)
bridge default_game_3() {
  std::array<std::string, 4> card_strings = {
      //
      "9dJd2s",  //
      "Qc3hKs",  //
      "7d7hKh",  //
      "4d5h6h"   //
  };
  std::array<hand, 4> cards;
  for (size_t i = 0; i < 4; ++i) {
    for (size_t j = 0; j < 3; ++j) {
      cards[i].push_back(card(card_strings[i].substr(j * 2, 2).c_str()));
    }
  }
  bridge game;
  game.deal(cards);
  return game;
}

// game results from create_random_skat_game(7)
golv::skat default_skat_game_7(int rotation, unsigned soloist,
                               bool skip_pushing)
{
  std::vector<std::string> card_strings = {
      //
      "8d 9d Ad 8h 7s 9s Tc",  //
      "Kd 8s Qs 8c Kc Jh Jc",  //
      "Td 9h Th Ks 7c 9c Ac",  //
      "As 7h"                  //
  };
  std::vector<hand> hands;
  for (size_t i = 0; i < card_strings.size(); ++i) {
    hands.push_back(golv::to_hand(card_strings[i]));
    std::cout << hands[i] << std::endl;
  }
  std::rotate(hands.begin(), hands.begin() + rotation,
              hands.begin() + skat::num_players);
  skat game;
  game.deal(hands[0], hands[1], hands[2], hands[3]);
  game.set_soloist(soloist);
  if (skip_pushing) {
    game.skip_pushing();
  }
  return game;
}

// game results from create_random_skat_game(5)
golv::skat default_skat_game_5(int rotation, unsigned soloist,
                               bool skip_pushing)
{
  std::vector<std::string> card_strings = {
      //
      "Th Ks 7c 9c Ac",  //
      "9d Td 8h 9h 9s",  //
      "8d Ad 7s Qs Tc",  //
      "8s Kd"            //
  };
  std::vector<hand> hands;
  for (size_t i = 0; i < card_strings.size(); ++i) {
    hands.push_back(golv::to_hand(card_strings[i]));
    std::cout << hands[i] << std::endl;
  }
  std::rotate(hands.begin(), hands.begin() + rotation,
              hands.begin() + skat::num_players);
  skat game;
  game.deal(hands[0], hands[1], hands[2], hands[3]);
  game.set_soloist(soloist);
  if (skip_pushing) {
    game.skip_pushing();
  }
  return game;
}

// game results from create_random_skat_game(10)
golv::skat default_skat_game_10(int rotation, unsigned soloist,
                                bool skip_pushing)
{
  std::vector<std::string> card_strings = {
      //
      "9dTd8h9hTh9sKs7c9cAc",  //
      "8dKdAd7s8sQs8cTcJhJc",  //
      "Qd7hQhKhAhAsQcKcJdJs",  //
      "7dTs"                   //
  };
  std::vector<hand> hands;
  for (size_t i = 0; i < card_strings.size(); ++i) {
    hands.push_back(golv::to_hand(card_strings[i]));
    std::cout << hands[i] << std::endl;
  }
  std::rotate(hands.begin(), hands.begin() + rotation,
              hands.begin() + skat::num_players);
  skat game;
  game.deal(hands[0], hands[1], hands[2], hands[3]);
  game.set_soloist(soloist);
  if (skip_pushing) {
    game.skip_pushing();
  }
  return game;
}