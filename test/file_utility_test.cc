#include <gtest/gtest.h>
#include <iostream>

#include "snapshot/snapshot.h"

using namespace std;
using namespace snapshot;

class FileUtilityTest : public testing::Test {
public:
};

TEST_F(FileUtilityTest, mkdir) {
    std::string s = "/tmp/340E2B12-25DD-4BB6-9A41-D24C8C4F76D4/a/b/c/d";

    FileUtility::Mkdir(s);

    EXPECT_EQ(0, access("/tmp/340E2B12-25DD-4BB6-9A41-D24C8C4F76D4/a", F_OK));
    EXPECT_EQ(0, access("/tmp/340E2B12-25DD-4BB6-9A41-D24C8C4F76D4/a/b", F_OK));
    EXPECT_EQ(0, access("/tmp/340E2B12-25DD-4BB6-9A41-D24C8C4F76D4/a/b/c", F_OK));
    EXPECT_EQ(0, access("/tmp/340E2B12-25DD-4BB6-9A41-D24C8C4F76D4/a/b/c/d", F_OK));

    system("rm -rf /tmp/340E2B12-25DD-4BB6-9A41-D24C8C4F76D4");
}

TEST_F(FileUtilityTest, remove_file) {
    const char* file_path = "/tmp/340E2B12-25DD-4BB6-9A41-D24C8C4F76D4";
    system((std::string("touch ") + file_path).c_str());

    EXPECT_EQ(0, access(file_path, F_OK));

    FileUtility::RemoveFile(file_path);

    EXPECT_NE(0, access(file_path, F_OK));
}
