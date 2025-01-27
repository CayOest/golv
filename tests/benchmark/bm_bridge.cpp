#include <chrono>
#include <golv/algorithm/alphabeta.hpp>
#include <golv/algorithm/mtd_f.hpp>
#include <golv/util/test_utils.hpp>
#include <iomanip>
#include <iostream>
using namespace golv;

namespace {

class Timer {
 public:
  Timer() { start_time_ = std::chrono::high_resolution_clock::now(); }

  ~Timer() {
    stop_time_ = std::chrono::high_resolution_clock::now();
    // auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop_time_ - start_time_).count();
  }

  auto stop() {
    stop_time_ = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop_time_ - start_time_).count();
    return duration;
  }

 private:
  std::chrono::high_resolution_clock::time_point start_time_;
  std::chrono::high_resolution_clock::time_point stop_time_;
};

auto test_alphabeta(bridge const& g) {
  Timer t;
  auto sol = golv::alphabeta(g);
  auto duration = t.stop();
  return duration;
}

auto test_alphabeta_with_memory(bridge const& g) {
  Timer t;
  auto sol = golv::alphabeta_with_memory(g);
  auto duration = t.stop();
  return duration;
}

auto test_mtd_f(bridge const& g) {
  Timer t;
  auto sol = golv::mtd_f(g).solve(5, 0, 13);
  auto duration = t.stop();
  return duration;
}

}  // namespace

int main() {
  int max_n = 10;
  golv::set_log_level(golv::log_level::debug);
  for (int n = 3; n < 8; ++n) {
    auto game = create_random_game(n);
    auto dur_ab = test_alphabeta(game) / 1000.0;
    auto dur_mem = test_alphabeta_with_memory(game) / 1000.0;
    auto dur_mtd = test_mtd_f(game) / 1000.0;
    std::cout << std::fixed << std::setprecision(2);
    std::cout << n << " = " << dur_ab << "  " << dur_mem << "  " << dur_mtd << std::endl;
  }
  for (int n = 8; n < max_n; ++n) {
    auto game = create_random_game(n);
    // auto dur_mem = test_alphabeta_with_memory(game) / 1000.0;
    auto dur_mtd = test_mtd_f(game) / 1000.0;
    std::cout << std::fixed << std::setprecision(2);
    // std::cout << n << " = " << dur_mem << "  " << dur_mtd << std::endl;
    std::cout << n << " = " << dur_mtd << std::endl;
  }

  return 0;
}