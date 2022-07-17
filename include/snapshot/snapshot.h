#ifndef SNAPSHOT_SNAPSHOT_H
#define SNAPSHOT_SNAPSHOT_H

#include <string.h>
#include <functional>
#include <iomanip>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include "./internal/file_utility.h"
#include "./internal/string_utility.h"
#include "./internal/system_utility.h"
#include "./internal/utility.h"

namespace snapshot {

class Snapshot {
    friend class SnapshotTest;

public:
    template <typename T>
    static void GenerateSnapshot(const T& t,
                                 const char* file_name,
                                 const char* func_name,
                                 const int line_number,
                                 const std::vector<std::string>& custom_keys = std::vector<std::string>({})) {
        const auto content = StringUtility::ToString(t);
        const auto filename_split = StringUtility::Split(file_name, '/');
        const auto snapshot_key = getSnapshotKey(file_name, func_name, line_number, custom_keys);
        const auto snapshot_filename = getSnapshotFilename(filename_split.back());
        const auto snapshot_dir = getSnapshotDir(filename_split);
        const auto snapshot_target_file = StringUtility::Join({snapshot_dir, snapshot_filename}, '/');

        if (isFirstEnter(snapshot_dir)) {
            FileUtility::RemoveDirectory(snapshot_dir);
        }

        if (!FileUtility::Mkdir(snapshot_dir)) {
            return;
        }

        std::string res;

        res += "// ";
        res += snapshot_key;
        res += "\n";
        res += content;
        res += "\n";

        FileUtility::AppendFile(snapshot_target_file, res);
    }

    template <typename T>
    static T GenerateSnapshotInline(const T& t, const char* file_name, const int line_number) {
        const auto content = StringUtility::ToString(t);
        auto file_content = FileUtility::GetAllLines(file_name);
        auto& line_number_vec = getLineNumberVec(file_name, static_cast<int>(file_content.size()));
        const auto actual_line_number = getActualLineNumber(line_number_vec, line_number);

        auto match_range = getSnapshotInlineMatchRange(file_content[actual_line_number - 1]);

        if (match_range == std::make_pair(-1, -1)) {
            return t;
        }

        file_content[actual_line_number - 1] =
                replaceSnapshotInlineContent(match_range, file_content[actual_line_number - 1], content);

        FileUtility::RewriteFile(file_name, StringUtility::Join(file_content, '\n'));

        line_number_vec[line_number - 1] += Utility::Count(file_content[actual_line_number - 1], [](const char c) {
            return c == '\n';
        });

        return t;
    }

    template <typename T>
    static void GenerateSnapshotDiff(const T& before,
                                     const T& after,
                                     const char* file_name,
                                     const char* func_name,
                                     const int line_number,
                                     const std::vector<std::string>& custom_keys = std::vector<std::string>({})) {
        func_name = fixFuncName(func_name);
        const auto filename_split = StringUtility::Split(file_name, '/');
        const auto snapshot_key = getSnapshotKey(file_name, func_name, line_number, custom_keys);
        const auto snapshot_filename = getSnapshotDiffFilename(filename_split.back());
        const auto snapshot_dir = getSnapshotDir(filename_split);
        const auto snapshot_diff_target_file = StringUtility::Join({snapshot_dir, snapshot_filename}, '/');

        if (isFirstEnter(snapshot_dir)) {
            FileUtility::RemoveDirectory(snapshot_dir);
        }

        if (!FileUtility::Mkdir(snapshot_dir)) {
            return;
        }

        const auto before_content = StringUtility::ToString(before);
        const auto after_content = StringUtility::ToString(after);
        const auto before_content_target_file = StringUtility::Join(
                {"/tmp", getSnapshotDiffBeforeFilename(StringUtility::Split(snapshot_key, '/').back())}, '/');
        const auto after_content_target_file = StringUtility::Join(
                {"/tmp", getSnapshotDiffAfterFilename(StringUtility::Split(snapshot_key, '/').back())}, '/');

        FileUtility::RewriteFile(before_content_target_file, before_content);
        FileUtility::RewriteFile(after_content_target_file, after_content);

        const auto diff_content = SystemUtility::Diff(before_content_target_file, after_content_target_file);

        FileUtility::RemoveFile(before_content_target_file);
        FileUtility::RemoveFile(after_content_target_file);

        std::string res;

        res += "// ";
        res += snapshot_key;
        res += "\n";
        res += "// before\n";
        res += before_content;
        res += "\n";
        res += "// after\n";
        res += after_content;
        res += "\n";
        res += "// diff\n";
        res += diff_content;

        FileUtility::AppendFile(snapshot_diff_target_file, res);
    }

private:
    static const char* fixFuncName(const char* func_name) {
        if (strcmp(func_name, "operator()") == 0) {
            return "operator";
        }

        return func_name;
    }

    static std::pair<int, int> getSnapshotInlineMatchRange(const std::string& s) {
        const std::string prefix = "SNAPSHOT_INLINE(";

        int len = static_cast<int>(s.length());
        int prefix_len = static_cast<int>(prefix.length());

        for (int i = static_cast<int>(prefix.length()) - 1; i + 1 < len; i++) {
            if (s.substr(i - prefix_len + 1, prefix_len) == prefix) {
                int l = 1;
                for (int j = i + 1; j < len; j++) {
                    if (s[j] == '(') {
                        ++l;
                    } else if (s[j] == ')') {
                        --l;
                    }

                    if (l == 0) {
                        return std::make_pair(i - prefix_len + 1, j);
                    }
                }
            }
        }

        return std::make_pair(-1, -1);
    }

    static std::string replaceSnapshotInlineContent(std::pair<int, int> replace_range,
                                                    const std::string& origin_s,
                                                    const std::string& replace_s) {
        return origin_s.substr(0, replace_range.first) + replace_s + origin_s.substr(replace_range.second + 1);
    }

    static std::string getSnapshotsDirname() {
        return "__snapshots__";
    }

    static std::string getSnapshotDir(const std::vector<std::string>& filename_split) {
        bool has_test = Utility::Any(filename_split, [](const std::string& s) {
            return s == "test";
        });

        auto filename_split_copy = filename_split;
        if (has_test) {
            while (!filename_split_copy.empty()) {
                if (filename_split_copy.back() != "test") {
                    filename_split_copy.pop_back();
                } else {
                    break;
                }
            }
        } else {
            filename_split_copy.pop_back();
        }

        filename_split_copy.push_back(getSnapshotsDirname());

        return StringUtility::Join(filename_split_copy, '/');
    }

    static std::string getSnapshotFilename(const std::string& file_name) {
        return std::string(file_name) + std::string(".snap.cc");
    }

    static std::string getSnapshotDiffFilename(const std::string& file_name) {
        return std::string(file_name) + std::string(".snap.diff");
    }

    static std::string getSnapshotDiffBeforeFilename(const std::string& file_name) {
        return std::string(file_name) + std::string(".snap.before");
    }

    static std::string getSnapshotDiffAfterFilename(const std::string& file_name) {
        return std::string(file_name) + std::string(".snap.after");
    }

    static std::string getSnapshotKey(const char* file_name,
                                      const char* func_name,
                                      const int line_number,
                                      const std::vector<std::string>& custom_keys) {
        std::string key = "";

        key += file_name;
        key += ".";
        key += func_name;
        key += ".";
        key += StringUtility::ToString(line_number);

        if (!custom_keys.empty()) {
            key += ".";
            key += StringUtility::Join(custom_keys, '.');
        }

        const int ix = count[key]++;

        key += ".";
        key += StringUtility::ToString(ix);

        return key;
    }

    static int getActualLineNumber(const std::vector<int>& line_number_vec, const int origin_line_number) {
        int res = 0;
        for (int i = 0; i < origin_line_number; i++) {
            res += line_number_vec[i];
        }

        return res;
    }

    static std::vector<int>& getLineNumberVec(const std::string& file_name, const int total_line_num) {
        if (line_number_map.count(file_name) == 0) {
            line_number_map[file_name] = std::vector<int>(total_line_num, 1);
        }

        return line_number_map[file_name];
    }

    static bool isFirstEnter(const std::string& key) {
        if (count[key]++ == 0) {
            return true;
        }

        return false;
    }

    inline static std::map<std::string, int> count;
    inline static std::map<std::string, std::vector<int>> line_number_map;
};

}  // namespace snapshot

#define SNAPSHOT(content, ...)            \
    snapshot::Snapshot::GenerateSnapshot( \
            content, __FILE__, __FUNCTION__, __LINE__, std::vector<std::string>({__VA_ARGS__}))

#define SNAPSHOT_INLINE(content) snapshot::Snapshot::GenerateSnapshotInline(content, __FILE__, __LINE__)

#define SNAPSHOT_DIFF(before, after, ...)     \
    snapshot::Snapshot::GenerateSnapshotDiff( \
            before, after, __FILE__, __FUNCTION__, __LINE__, std::vector<std::string>({__VA_ARGS__}))

#endif  // SNAPSHOT_SNAPSHOT_H
