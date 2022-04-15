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

TEST_F(SystemUtilityTest, diff) {
    const char* file_a_path = "/tmp/22220693-3FBD-4A2A-A97A-15A181CC7A67";
    const char* file_b_path = "/tmp/22220693-3FBD-4A2A-A97A-15A181CC7A68";

    system((std::string("echo \"aaa\" > ") + file_a_path).c_str());
    system((std::string("echo \"bbb\" > ") + file_b_path).c_str());

    auto diff = SystemUtility::Diff(file_a_path, file_b_path);

    EXPECT_EQ(diff,
            std::string(
                    R"(diff --git a/tmp/22220693-3FBD-4A2A-A97A-15A181CC7A67 b/tmp/22220693-3FBD-4A2A-A97A-15A181CC7A68
index 72943a1..f761ec1 100644
--- a/tmp/22220693-3FBD-4A2A-A97A-15A181CC7A67
+++ b/tmp/22220693-3FBD-4A2A-A97A-15A181CC7A68
@@ -1 +1 @@
-aaa
+bbb
)"));

    system((std::string("rm ") + file_a_path).c_str());
    system((std::string("rm ") + file_b_path).c_str());
}
