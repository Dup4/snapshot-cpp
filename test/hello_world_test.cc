#include <gtest/gtest.h>

#include "snapshot-cpp/hello_world.hpp"

TEST(HELLO_WORLD, hello_world) {
    EXPECT_EQ(HelloWorld(), "Hello World");
}
