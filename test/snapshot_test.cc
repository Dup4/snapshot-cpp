#include <gtest/gtest.h>
#include <iostream>

#include "snapshot/snapshot.h"

using namespace std;
using namespace snapshot;

class SnapshotTest : public testing::Test, public Snapshot {
public:
};

TEST_F(SnapshotTest, to_string_int) {
    EXPECT_EQ(StringUtility::ToString(int(1)), "1");
}
