#include <golv/games/skat.hpp>
#include <golv/algorithms/mws.hpp>
#include <golv/util/test_utils.hpp>
#include <iostream>
#include <algorithm>
#include <future>

namespace {

auto generate_games(golv::skat game)
{
  std::vector<golv::skat> games;
  auto legal = game.legal_actions();
  for (size_t i = 0; i < legal.size() - 1; ++i) {
    game.apply_action(legal[i]);
    for (size_t j = i + 1; j < legal.size(); ++j) {
      game.apply_action(legal[j]);
      games.push_back(game);
      game.undo_action(legal[j]);
    }
    game.undo_action(legal[i]);
  }
  return games;
}

void list_options(golv::skat g)
{
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

  std::cout << g << std::endl;
  std::cout << "Set soloist [0, 1, 2]" << std::endl;
  std::string soloist;
  std::getline(std::cin, soloist);
  int s = 0;
  try {
    s = std::stoi(soloist);
  } catch (std::exception const&) {
    std::cout << "Error parsing soloist: " << soloist
              << ". Soloist set to 0 = Forehand." << std::endl;
  }
  g.set_soloist(s);
  std::cout << "Generating options" << std::endl;
  auto games = generate_games(g);
  std::vector<std::future<golv::skat::value_type>> futures;
  std::vector<golv::hand> options;
  std::cout << "Calculating options" << std::endl;
  for (auto g : games) {
    options.push_back(g.blinds());
    auto fun = [g]() { return golv::mws_binary_search<golv::skat>(g).first; };
    futures.push_back(std::async(std::launch::async, fun));
  }
  std::vector<std::pair<golv::hand, golv::skat::value_type>> values;
  for (size_t i = 0; i < futures.size(); ++i) {
    values.push_back({options[i], futures[i].get()});
  }
  std::sort(values.begin(), values.end(),
            [](auto const& left, auto const& right) {
              return left.second > right.second;
            });
  auto max_value = values.front().second;
  std::cout << "Max Value = " << max_value << std::endl;
  std::cout << "Best Push Options = " << std::endl;
  auto it = std::find_if(
      values.begin(), values.end(),
      [max_value](auto const& pr) { return pr.second != max_value; });
  for (auto it2 = values.begin(); it2 != it; ++it2) {
    std::cout << it2->first << std::endl;
  }
  std::cout << "All Push Options = " << std::endl;
  for (auto const& pr : values) {
    std::cout << pr.first << " : " << pr.second << std::endl;
  }
}

golv::hand get_skat(std::array<golv::hand, 4> distribution)
{
  auto deck = golv::create_skat_deck();
  for (int i = 0; i < 3; ++i) {
    std::cout << distribution[i].size() << std::endl;
    for (auto c : distribution[i]) {
      auto it = std::find(deck.begin(), deck.end(), c);
      if (it == deck.end())
        throw std::domain_error("Error: " + golv::to_string(c) +
                                " occurs twice.");  // TODO
      else {
        deck.erase(it);
      }
    }
  }
  if (deck.size() != 2)
    throw std::domain_error("Error: " + std::to_string(deck.size()) +
                            " remaining cards.");

  return deck;
}

golv::skat make_game(std::array<golv::hand, 4> dist)
{
  golv::skat game;
  dist[3] = get_skat(dist);
  game.deal(dist[0], dist[1], dist[2], dist[3]);
  return game;
}

void insert_hand()
{
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

  list_options(game);
}

void generate_random_game()
{
  std::cout << "Generating a random game" << std::endl << std::endl;
  auto game = create_random_skat_game(10, 0, 0);
  list_options(game);
}

}  // namespace

int main()
{
  std::cout << "Optimal Skat Pusher" << std::endl;
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