#include <gtest/gtest.h>

#include "snapshot/snapshot.h"

using namespace snapshot;

class SnapshotTest : public testing::Test {};

TEST_F(SnapshotTest, to_string_int) {
    EXPECT_EQ(StringUtility::ToString(int(1)), "1");
}
