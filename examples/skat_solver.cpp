#include <golv/games/skat.hpp>
#include <golv/algorithms/mws.hpp>
#include <golv/util/test_utils.hpp>
#include <iostream>
#include <algorithm>

namespace {

void walk_through_game(golv::skat game) {
  std::cout << game << std::endl;

  while (!game.is_terminal()) {
    std::cout << "The options are: " << std::endl;
    auto legal = game.legal_actions();
    for (size_t i = 0; i < legal.size(); ++i) {
      std::cout << legal[i] << " [" << i << "] ";
      game.apply_action(legal[i]);
      auto [value, bm] = golv::mws_binary_search(game);
      game.undo_action(legal[i]);
      std::cout << value << std::endl;
    }
    std::cout << "Choose an option: ";
    int option;
    std::cin >> option;
    game.apply_action(legal[option]);
  }
}

golv::hand get_skat(std::array<golv::hand, 4> distribution) {
  auto deck = golv::create_skat_deck();
  for (int i = 0; i < 3; ++i) {
    std::cout << distribution[i].size() << std::endl;
    for (auto c : distribution[i]) {
      auto it = std::find(deck.begin(), deck.end(), c);
      if (it == deck.end())
        throw std::domain_error("Error: " + golv::to_string(c) + " occurs twice.");  // TODO
      else {
        deck.erase(it);
      }
    }
  }
  if (deck.size() != 2) throw std::domain_error("Error: " + std::to_string(deck.size()) + " remaining cards.");

  return deck;
}

golv::skat make_game(std::array<golv::hand, 4> distribution) {
  golv::skat game;
  distribution[3] = get_skat(distribution);
  game.deal(distribution);
  return game;
}

void insert_hand() {
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  std::cout << "Inserting a hand" << std::endl;
  std::cout << "Example hand: JcJs AcTc 9s8s Kh9h8h7h" << std::endl;
  std::cout << "FOREHAND" << std::endl;
  std::string fh_hand;
  std::getline(std::cin, fh_hand);
  auto fh_cards = golv::to_hand(fh_hand);
  assert(fh_cards.size() == 10);

  std::cout << "MIDDLEHAND" << std::endl;
  std::string mh_hand;
  std::getline(std::cin, mh_hand);
  auto mh_cards = golv::to_hand(mh_hand);
  assert(mh_cards.size() == 10);

  std::cout << "BACKHAND" << std::endl;
  std::string bh_hand;
  std::getline(std::cin, bh_hand);
  auto bh_cards = golv::to_hand(bh_hand);
  assert(bh_cards.size() == 10);

  std::array<golv::hand, 4> dist;
  dist[0] = fh_cards;
  dist[1] = mh_cards;
  dist[2] = bh_cards;
  auto game = make_game(dist);

  std::cout << "SOLOIST [0, 1, 2]" << std::endl;
  std::string soloist;
  std::getline(std::cin, soloist);
  int s = 0;
  try {
    s = std::stoi(soloist);
  } catch (std::exception const&) {
    std::cout << "Error parsing soloist: " << soloist << ". Soloist set to 0 = Forehand." << std::endl;
  }
  game.set_soloist(s);
  walk_through_game(game);
}

void generate_random_game() {
  std::cout << "Generating a random game" << std::endl << std::endl;
  auto game = create_random_skat_game(10, 0, 0);
  walk_through_game(game);
}

}  // namespace

int main() {
  std::cout << "golv Skat Solver" << std::endl;
  std::cout << "----------------" << std::endl << std::endl;
  std::cout << "Options:" << std::endl;

  int choice = 0;

  golv::skat game;
  while (choice != 1 && choice != 2) {
    std::cout << "[1] Insert hand" << std::endl;
    std::cout << "[2] Generate random game" << std::endl << std::endl;
    std::cout << "Your choice: ";
    std::cin >> choice;
    if (!std::cin.good()) {
      choice = 0;
      std::cin.clear();
      std::cin.ignore(10000, '\n');
      std::cout << "Wrong option: [1] or [2]." << std::endl;
      continue;
    }
    switch (choice) {
      case 1:
        insert_hand();
        break;
      case 2:
        generate_random_game();
        break;
      default:
        break;
    }
  }
}