#include <gtest/gtest.h>

#include <algorithm>
#include <functional>
#include <golv/algorithm/cfr.hpp>
#include <golv/games/rps.hpp>
#include <golv/util/logging.hpp>
#include <random>

using namespace golv;

std::string print(std::vector<double> const& v) {}

TEST(cfr, rps) {
  golv::set_log_level(golv::log_level::debug);
  rock_paper_scissors game;
  cfr solver(game);
  auto val = solver.solve(10000000);
  GOLV_LOG_INFO("val = " << val);
  auto const& map = solver.map();
  for (auto const& pr : map) {
    std::cout << "state = " << pr.first << " -> strategy = ";
    auto strat = pr.second.avg_strategy();
    std::copy(std::begin(strat), std::end(strat), std::ostream_iterator<double>(std::cout, ", "));
    std::cout << std::endl;
  }
}