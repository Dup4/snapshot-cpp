#include <gtest/gtest.h>
#include <iostream>

#include "snapshot/snapshot.h"

using namespace std;
using namespace snapshot;

class SnapshotTest : public testing::Test, public Snapshot {
public:
};

class RawString {
public:
    std::string s;

    friend ostream& operator<<(ostream& os, const RawString& r) {
        os << r.s;
        return os;
    }
};

TEST_F(SnapshotTest, to_string_int) {
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

    SNAPSHOT_INLINE(r);

    SNAPSHOT_DIFF(a, b);
}
