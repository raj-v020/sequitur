#include <gtest/gtest.h>
#include <sequitur/core/MatchingEngine.hpp>

using namespace sequitur::core;

TEST(MatchingEngineTest, BlackBoxExecution) {
  MatchingEngine engine(100);
  engine.submit_order(1, 150, 100);
  engine.submit_order(1, 151, 100);
  engine.submit_order(0, 155, 250);
}
