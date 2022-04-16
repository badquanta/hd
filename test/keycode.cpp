#include <gtest/gtest.h>

#include <hdEvents.hpp>
// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions) {
  hd::sdl::event::key::CodeDispatcher keyDispatcher;
  testing::MockFunction<
  // Expect two strings not to be equal.
  EXPECT_STRNE("hello", "world");
  // Expect equality.
  EXPECT_EQ(7 * 6, 42);
}