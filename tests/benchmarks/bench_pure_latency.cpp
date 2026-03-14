#include <cstddef>
#include <cstdint>
#include <iostream>
#include <sequitur/core/MatchingEngine.hpp>
#include <sequitur/utils/Timer.hpp>

#define POOL_CAP 1'000'000

using namespace sequitur::core;
using namespace sequitur::utils;
using namespace std;

int main() {
  MatchingEngine engine(POOL_CAP);
  std::vector<uint64_t> latency(POOL_CAP);
  uint64_t idx = 0;
  for (size_t i = 0; 2 * i < POOL_CAP; i++) {
    {
      Timer t(latency.data(), &idx);
      engine.submit_order(0, 100, 10);
    }
    {
      Timer t(latency.data(), &idx);
      engine.submit_order(1, 100, 10);
    }
  }

  cout << "Total Orders Processed: " << engine.get_total_orders() << endl;
  cout << "Total Trades Executed: " << engine.get_total_trades() << endl;
  cout << "Total Volume Matched: " << engine.get_total_volume() << endl;

  uint64_t total_ns = 0;
  for (uint64_t i = 0; i < idx; i++) {
    total_ns += latency[i];
  }
  cout << "Total Nanoseconds: " << total_ns << endl;

  double total_s = total_ns / 1'000'000'000.0;
  cout << "Total Seconds: " << total_s << endl;

  double avg_latency = static_cast<double>(total_ns) / POOL_CAP;

  double throughput = POOL_CAP / total_s;

  cout << "Average Latency: " << avg_latency << endl;
  cout << "Throughput: " << throughput << endl;
  return 0;
}
