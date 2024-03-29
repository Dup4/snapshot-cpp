#include <gtest/gtest.h>
#include <iostream>

#include "snapshot/snapshot.h"

using namespace std;

namespace snapshot {

class SnapshotTest : public testing::Test {
public:
    static std::pair<int, int> getSnapshotInlineMatchRange(const std::string& s) {
        return Snapshot::getSnapshotInlineMatchRange(s);
    }
};

class RawString {
public:
    std::string s;

    friend ostream& operator<<(ostream& os, const RawString& r) {
        os << r.s;
        return os;
    }
};

TEST_F(SnapshotTest, example) {
    const std::string a = R"({
        "foo": bar
})";

    const std::string b = R"({
        "foo": bar2
})";

    SNAPSHOT(a);

    const char* snapshot_inline = "SNAPSHOT_INLINE(r)";

    RawString r;
    r.s = snapshot_inline;

    SNAPSHOT(r);

    SNAPSHOT_DIFF(a, b);
}

TEST_F(SnapshotTest, get_snapshot_inline_match_range) {
    // this case will be broken
    EXPECT_EQ(SnapshotTest::getSnapshotInlineMatchRange("SNAPSHOT_INLINE(\")\")"), make_pair(0, 17));
}

TEST_F(SnapshotTest, README) {
    const std::string tmp = "snapshot-cpp";
    const std::string tmp_diff = tmp + "-diff";

    EXPECT_EQ(tmp, std::string("snapshot-cpp"));
    SNAPSHOT(tmp);
    SNAPSHOT_DIFF(tmp, tmp_diff);
}

}  // namespace snapshot
