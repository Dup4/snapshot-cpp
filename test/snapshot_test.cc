#include <gtest/gtest.h>

#include "snapshot/snapshot.h"

class SnapshotTest : public testing::Test {};

TEST_F(SnapshotTest, to_string_int) {
    EXPECT_EQ(snapshot::Snapshot::ToString(int(1)), "1");
}
