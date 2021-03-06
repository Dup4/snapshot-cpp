#include <gtest/gtest.h>
#include <iostream>

#include "snapshot/snapshot.h"

using namespace std;
using namespace snapshot;

class UtilityTest : public testing::Test {
public:
};

TEST_F(UtilityTest, all_and_any_and_count) {
    vector<int> v({1, 2, 3});

    EXPECT_EQ(true, Utility::All(v, [](const int x) {
                  return x >= 1;
              }));

    EXPECT_EQ(true, Utility::Any(v, [](const int x) {
                  return x >= 2;
              }));

    EXPECT_EQ(2, Utility::Count(v, [](const int x) {
                  return x >= 2;
              }));
}
