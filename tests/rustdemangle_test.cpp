
#include "rustdemangle.h"

#include "gtest/gtest.h"

TEST(rustdemangle, greenWIP) {
  std::string input = "_ZN4testE";

  std::string output = rustdemangle(input);

  ASSERT_EQ(output, "test");
}
