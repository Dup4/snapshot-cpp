#include <gtest/gtest.h>
#include <iostream>

#include "snapshot/snapshot.h"

using namespace std;
using namespace snapshot;

class StringUtilityTest : public testing::Test {
public:
};

TEST_F(StringUtilityTest, to_string_string) {
    std::string s1 = R"("foo": bar)";

    EXPECT_EQ(s1, std::string(R"("foo": bar)"));
}
