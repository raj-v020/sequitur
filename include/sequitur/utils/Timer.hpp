#pragma once

#include <chrono>
#include <cstdint>

namespace sequitur {
namespace utils {
class Timer {
private:
#ifdef ENABLE_TELEMETRY
  std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
  uint64_t *bfr, *idx;

#endif
public:
  Timer([[maybe_unused]] uint64_t *buffer, [[maybe_unused]] uint64_t *index) {
#ifdef ENABLE_TELEMETRY
    start_time = std::chrono::high_resolution_clock::now();
    bfr = buffer;
    idx = index;
#endif
  }
  ~Timer() {
#ifdef ENABLE_TELEMETRY
    auto end_time = std::chrono::high_resolution_clock::now();

    auto interval = std::chrono::duration_cast<std::chrono::nanoseconds>(
        end_time - start_time);

    bfr[(*idx)++] = interval.count();
#endif
  }
};
} // namespace utils
} // namespace sequitur
