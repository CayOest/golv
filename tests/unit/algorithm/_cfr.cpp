#include <gtest/gtest.h>

#include <algorithm>
#include <functional>
#include <golv/algorithm/cfr.hpp>
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