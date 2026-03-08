#include <gtest/gtest.h>
#include <sequitur/core/Order.hpp>

TEST(OrderStructTest, test_size_and_align) {
  EXPECT_EQ(sizeof(sequitur::core::Order), 64);
  EXPECT_EQ(alignof(sequitur::core::Order), 64);
}
