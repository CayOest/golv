#include <gtest/gtest.h>

#include <algorithm>
#include <functional>
#include <golv/algorithm/cfr.hpp>
#include <golv/games/kuhn.hpp>
#include <golv/games/rps.hpp>
#include <golv/util/logging.hpp>
#include <random>

using namespace golv;

TEST(cfr, rps) {
  golv::set_log_level(golv::log_level::debug);
  rock_paper_scissors game;
  cfr solver(game);
  auto val = solver.solve();
  GOLV_LOG_INFO("val = " << val);
  // value should be -1
  EXPECT_NEAR(val, -1.0, 0.01);

  auto const& map = solver.map();

  // Initial state strategy output
  {
    auto initial_strat = map.at("").avg_strategy();
    GOLV_LOG_DEBUG("Initial strategy:");
    for (auto const& s : initial_strat) {
      GOLV_LOG_DEBUG(s);
    }
  }

  // 'p' state strategy check
  {
    auto p_strat = map.at("p").avg_strategy();
    EXPECT_NEAR(p_strat[0], 0.0, 0.01);
    EXPECT_NEAR(p_strat[1], 0.0, 0.01);
    EXPECT_NEAR(p_strat[2], 1.0, 0.01);
  }

  // 'r' state strategy check
  {
    auto r_strat = map.at("r").avg_strategy();
    EXPECT_NEAR(r_strat[0], 0.0, 0.01);
    EXPECT_NEAR(r_strat[1], 1.0, 0.01);
    EXPECT_NEAR(r_strat[2], 0.0, 0.01);
  }

  // 's' state strategy check
  {
    auto s_strat = map.at("s").avg_strategy();
    EXPECT_NEAR(s_strat[0], 1.0, 0.01);
    EXPECT_NEAR(s_strat[1], 0.0, 0.01);
    EXPECT_NEAR(s_strat[2], 0.0, 0.01);
  }
}

TEST(cfr, kuhn) {
  golv::set_log_level(golv::log_level::debug);
  kuhn game;
  cfr solver(game);
  auto val = solver.solve(100000);
  GOLV_LOG_DEBUG("val = " << val);
  double expected_val = 17.0 / 18.0 - 1.0;
  EXPECT_NEAR(val, expected_val, 0.1);
  auto const& strategy = solver.map();
  for (auto const& [info_set, node] : strategy) {
    GOLV_LOG_DEBUG(info_set);
    auto avg_strat = node.avg_strategy();
    for (size_t i = 0; i < node.size(); ++i) {
      GOLV_LOG_DEBUG(node.actions[i] << ": " << avg_strat[i]);
    }
  }

  // plausi check 1: betting with 0 is 1/3 the frequency of betting with 1
  auto x = solver.map().at("0|").avg_strategy()[1];
  auto y = solver.map().at("2|").avg_strategy()[1];
  EXPECT_NEAR(x, y / 3, 0.1);

  // plausi check 2: Player 2 calls with 1 with frequency 1/3
  auto freqCall1 = solver.map().at("1|b").avg_strategy()[1];
  EXPECT_NEAR(freqCall1, 1.0 / 3.0, 0.1);

  // plausi check 3: Player 2 bets with 0 after Player 1 checks: 1/3
  auto freqBet0 = solver.map().at("0|x").avg_strategy()[1];
  EXPECT_NEAR(freqBet0, 1.0 / 3.0, 0.1);

  // plausi check 4: Player 1 calls to Player 2's bet with 1: (y+1)/3
  freqCall1 = solver.map().at("1|xb").avg_strategy()[1];
  EXPECT_NEAR(freqCall1, (y + 1.0) / 3.0, 0.1);
}