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

TEST_F(FileUtilityTest, remove_dir) {
    const char* file_path = "/tmp/340E2B12-25DD-4BB6-9A41-D24C8C4F76D4/a/b/c";

    FileUtility::Mkdir(file_path);

    EXPECT_EQ(0, access(file_path, F_OK));

    FileUtility::RemoveDirectory("/tmp/340E2B12-25DD-4BB6-9A41-D24C8C4F76D4");

    EXPECT_NE(0, access(file_path, F_OK));
}

TEST_F(FileUtilityTest, get_all_lines) {
    const char* file_path = "/tmp/340E2B12-25DD-4BB6-9A41-D24C8C4F76D4";

    system((std::string("echo \"a\nb\nc\" > ") + file_path).c_str());

    const auto v = FileUtility::GetAllLines(file_path);

    EXPECT_EQ(4, v.size());
    EXPECT_EQ(v[0], "a");
    EXPECT_EQ(v[1], "b");
    EXPECT_EQ(v[2], "c");
    EXPECT_EQ(v[3], "");

    system((std::string("rm ") + file_path).c_str());
}

TEST_F(FileUtilityTest, rewrite_file) {
    const char* file_path = "/tmp/340E2B12-25DD-4BB6-9A41-D24C8C4F76D4";

    system((std::string("echo \"abc\" > ") + file_path).c_str());

    EXPECT_EQ(StringUtility::Join(FileUtility::GetAllLines(file_path), ' '), "abc ");

    FileUtility::RewriteFile(file_path, "cba");

    EXPECT_EQ(StringUtility::Join(FileUtility::GetAllLines(file_path), ' '), "cba");

    system((std::string("rm ") + file_path).c_str());
}
