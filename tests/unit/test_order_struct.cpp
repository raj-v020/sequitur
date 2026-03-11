#include <gtest/gtest.h>
#include <sequitur/core/Order.hpp>

using namespace sequitur::core;

TEST(OrderStructTest, test_size_and_align) {
  EXPECT_EQ(sizeof(Order), 64);
  EXPECT_EQ(alignof(Order), 64);
}
