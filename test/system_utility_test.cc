#include <gtest/gtest.h>
#include <iostream>

#include "snapshot/snapshot.h"

using namespace std;
using namespace snapshot;

class SystemUtilityTest : public testing::Test {
public:
};

TEST_F(SystemUtilityTest, exec) {
    EXPECT_EQ(SystemUtility::Exec("echo \"aaa\""), "aaa\n");
}
