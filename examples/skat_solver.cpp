#include <golv/games/skat.hpp>
#include <golv/algorithms/mws.hpp>
#include <golv/util/test_utils.hpp>
#include <iostream>

namespace {
void insert_hand() {
  // TODO!
  std::cout << "Inserting a hand" << std::endl;
}

void generate_random_game() {
  std::cout << "Generating a random game" << std::endl << std::endl;
  auto game = create_random_skat_game(10, 0, 0);
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