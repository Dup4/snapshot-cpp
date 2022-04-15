#include <gtest/gtest.h>
#include <iostream>

#include "snapshot/snapshot.h"

using namespace std;
using namespace snapshot;

class FileUtilityTest : public testing::Test {
public:
};

TEST_F(FileUtilityTest, get_all_lines) {
    const std::string a = R"({
        "foo": bar
})";

    const std::string b = R"({
        "foo": bar2
})";

    SNAPSHOT_DIFF(a, b);
}
