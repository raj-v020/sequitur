#include <chrono>
#include <iostream>

namespace sequitur {
namespace utils {
class Timer {
private:
  std::chrono::time_point<std::chrono::high_resolution_clock> start_time;

public:
  Timer() : start_time(std::chrono::high_resolution_clock::now()) {}

  ~Timer() {
    auto end_time = std::chrono::high_resolution_clock::now();

    auto interval = std::chrono::duration_cast<std::chrono::nanoseconds>(
        end_time - start_time);

    std::cout << interval.count() << std::endl;
  }
};
} // namespace utils
} // namespace sequitur
