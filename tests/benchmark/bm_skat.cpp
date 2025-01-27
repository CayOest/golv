#include <chrono>
#include <golv/algorithm/alphabeta.hpp>
#include <golv/algorithm/mtd_f.hpp>
#include <golv/util/test_utils.hpp>
#include <iomanip>
#include <iostream>

#include "timer.hpp"

using namespace golv;

auto test_alphabeta(skat const& g) {
  Timer t;
  golv::alphabeta(g);
  auto duration = t.stop();
  return duration;
}

auto test_alphabeta_with_memory(skat const& g) {
  Timer t;
  golv::alphabeta_with_memory(g);
  auto duration = t.stop();
  return duration;
}

auto test_mtd_f(skat const& g) {
  Timer t;
  golv::mtd_f(g).solve(5, 0, 13);
  auto duration = t.stop();
  return duration;
}

int main() {
  int max_n = 5;
  golv::set_log_level(golv::log_level::error);
  for (int n = 3; n <= std::min(7, max_n); ++n) {
    auto game = create_random_skat_game(n, 2);
    auto dur_ab = test_alphabeta(game) / 1000.0;
    auto dur_mem = test_alphabeta_with_memory(game) / 1000.0;
    auto dur_mtd = test_mtd_f(game) / 1000.0;
    std::cout << std::fixed << std::setprecision(2);
    std::cout << n << " = " << dur_ab << "  " << dur_mem << "  " << dur_mtd << std::endl;
  }
  for (int n = 8; n <= max_n; ++n) {
    auto game = create_random_skat_game(n, 2);
    auto dur_mtd = test_mtd_f(game) / 1000.0;
    std::cout << std::fixed << std::setprecision(2);
    std::cout << n << " = " << dur_mtd << std::endl;
  }

  return 0;
}