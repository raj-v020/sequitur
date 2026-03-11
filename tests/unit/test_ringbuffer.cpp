#include <cstdint>
#include <gtest/gtest.h>
#include <sequitur/concurrency/RingBuffer.hpp>
#include <thread>
#include <vector>

using namespace sequitur::concurrency;

TEST(RingBufferTest, MultithreadedStressTest) {
  RingBuffer<uint64_t, 1024> buffer;
  const uint64_t MESSAGES = 1'000'000;
  std::thread prod([&]() {
    for (uint64_t i = 0; i < MESSAGES; i++) {
      while (!buffer.push(i))
        ;
    }
  });
  std::thread cons([&]() {
    for (uint64_t i = 0; i < MESSAGES; i++) {
      uint64_t item;
      while (!buffer.pop(item))
        ;
      EXPECT_EQ(item, i);
    }
  });

  prod.join();
  cons.join();
}
