#include <chrono>

class Timer {
 public:
  Timer() { start_time_ = std::chrono::high_resolution_clock::now(); }

  auto stop() {
    stop_time_ = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop_time_ - start_time_).count();
    return duration;
  }

 private:
  std::chrono::high_resolution_clock::time_point start_time_;
  std::chrono::high_resolution_clock::time_point stop_time_;
};