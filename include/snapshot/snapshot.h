#ifndef SNAPSHOT_SNAPSHOT_H
#define SNAPSHOT_SNAPSHOT_H

#include <ctype.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <array>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

namespace snapshot {

class Utility {
public:
    template <typename T, typename F>
    static bool All(const std::vector<T>& vec, const F&& func) {
        for (const auto& t : vec) {
            if (!func(t)) {
                return false;
            }
        }

        return true;
    }

    template <typename T, typename F>
    static bool Any(const std::vector<T>& vec, const F&& func) {
        for (const auto& t : vec) {
            if (func(t)) {
                return true;
            }
        }

        return false;
    }

    template <typename T, typename F>
    static int Count(const T& vec, const F&& func) {
        int res = 0;

        for (const auto& t : vec) {
            res += func(t);
        }

        return res;
    }
};

class StringUtility {
public:
    template <typename T>
    static std::string ToString(const T& t) {
        std::stringstream ss;
        std::string s;
        ss << t;
        ss >> s;
        return ss.str();
    }

    static std::string ToString(int value) {
        return std::to_string(value);
    }

    static std::string ToString(long value) {
        return std::to_string(value);
    }

    static std::string ToString(long long value) {
        return std::to_string(value);
    }

    static std::string ToString(unsigned value) {
        return std::to_string(value);
    }

    static std::string ToString(unsigned long value) {
        return std::to_string(value);
    }

    static std::string ToString(unsigned long long value) {
        return std::to_string(value);
    }

    static std::string ToString(float value) {
        return std::to_string(value);
    }

    static std::string ToString(double value) {
        return std::to_string(value);
    }

    static std::string ToString(long double value) {
        return std::to_string(value);
    }

    static std::string ToString(bool value) {
        return value ? "true" : "false";
    }

    static std::string ToString(const char* s) {
        if (hasEscapeCharacter(s)) {
            return std::string("R\"(") + s + std::string(")\"");
        } else {
            return std::string("\"") + s + std::string("\"");
        }
    }

    static std::string ToString(const std::string& s) {
        return std::string("std::string(") + ToString(s.c_str()) + std::string(")");
    }

    template <typename A, typename B>
    static std::string ToString(const std::pair<A, B>& v) {
        std::string res = "{";
        res += ToString(v.first);
        res += ", ";
        res += ToString(v.second);
        res += "}";
        return res;
    }

    template <typename T>
    static std::string ToString(const std::vector<T>& v) {
        if (v.empty()) {
            return "{}";
        }

        std::vector<std::string> string_vec = getStringVector(v);
        char delimiter = ',';

        std::string res = "{\n";

        for (const auto& s : string_vec) {
            res += "    ";
            res += s;
            res += delimiter;
            res += "\n";
        }

        res += "}\n";

        return res;
    }

    static std::vector<std::string> Split(const std::string& s, const char delimiter) {
        std::vector<std::string> res;
        int len = s.length();

        std::string t = "";
        for (int i = 0; i <= len; i++) {
            if (i == len || s[i] == delimiter) {
                res.push_back(t);
                t = "";
            } else {
                t += s[i];
            }
        }

        return res;
    }

    static std::string Join(const std::vector<std::string>& ss, const char delimiter) {
        std::string res = "";
        for (size_t i = 0; i < ss.size(); i++) {
            if (i) {
                res += delimiter;
            }

            res += ss[i];
        }

        return res;
    }

private:
    template <typename T>
    static std::vector<std::string> getStringVector(const std::vector<T>& vec) {
        std::vector<std::string> res;
        for (const auto& t : vec) {
            res.push_back(ToString(t));
        }

        return res;
    }

    static bool hasEscapeCharacter(const std::string& s) {
        const char* escape_character_set = "\\\"\'\?\a\b\f\n\r\t\v\0";

        const auto check = [&escape_character_set](char c) -> bool {
            auto p = escape_character_set;
            while (*p) {
                if (c == *p) {
                    return true;
                }
                ++p;
            }

            return false;
        };

        for (const auto& c : s) {
            if (check(c)) {
                return true;
            }
        }

        return false;
    }
};

class FileUtility {
public:
    // Create folder
    // If the directory does not exist, it will be created recursively
    static bool Mkdir(const std::string& path) {
        // if the path exists in its entirety
        // there is no need to create it
        if (access(path.c_str(), F_OK) == 0) {
            return true;
        }

        if (path.length() == 0) {
            return false;
        }

        size_t len = path.length();
        size_t idx = 0;
        int pos = -1;

        if (path[0] == '/') {
            ++pos;
        }

        auto __mkdir = [](const std::string& path) -> bool {
            if (access(path.c_str(), F_OK) == 0) {
                return true;
            }

            auto ret = mkdir(path.c_str(), S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);

            // if the path already exists
            // don't conside creation failed
            if (ret == 0 || errno == EEXIST) {
                return true;
            }

            return false;
        };

        while (true) {
            size_t cur_pos = path.find('/', pos + 1);
            pos = cur_pos;

            if (cur_pos == std::string::npos) {
                break;
            }

            auto _path = path.substr(0, pos);

            if (!__mkdir(_path)) {
                return false;
            }

            idx = pos + 1;
        }

        if (idx < len) {
            if (!__mkdir(path)) {
                return false;
            }
        }

        return true;
    };

    static int RemoveFile(const std::string& path) {
        return removeFile(path.c_str());
    }

    static int RemoveDirectory(const std::string& path) {
        return removeDirectory(path.c_str());
    }

    static std::vector<std::string> GetAllLines(const std::string& file_path) {
        std::vector<std::string> res;
        std::string tmp_s;

        std::ifstream read_file(file_path);

        while (!read_file.eof()) {
            getline(read_file, tmp_s);
            res.push_back(tmp_s);
        }

        read_file.close();
        return res;
    }

    static void RewriteFile(const std::string& file_path, const std::string& content) {
        std::ofstream file;
        file.open(file_path, std::ofstream::out | std::ofstream::trunc);
        file.write(content.c_str(), content.length());
        file.close();
    }

    static void AppendFile(const std::string& file_path, const std::string& content) {
        std::ofstream file;
        file.open(file_path, std::ofstream::out | std::ofstream::ate | std::ofstream::app);
        file.write(content.c_str(), content.length());
        file.close();
    }

private:
    static int removeFile(const char* path) {
        if (access(path, F_OK) != 0) {
            return 0;
        }

        return remove(path);
    }

    static int removeDirectory(const char* path) {
        DIR* d = opendir(path);
        size_t path_len = strlen(path);
        int r = -1;

        if (d) {
            struct dirent* p;

            r = 0;
            while (!r && (p = readdir(d))) {
                int r2 = -1;
                char* buf;
                size_t len;

                /* Skip the names "." and ".." as we don't want to recurse on them. */
                if (!strcmp(p->d_name, ".") || !strcmp(p->d_name, "..")) {
                    continue;
                }

                len = path_len + strlen(p->d_name) + 2;
                buf = (char*)malloc(len);

                if (buf) {
                    struct stat statbuf;

                    snprintf(buf, len, "%s/%s", path, p->d_name);
                    if (!stat(buf, &statbuf)) {
                        if (S_ISDIR(statbuf.st_mode)) {
                            r2 = removeDirectory(buf);
                        } else {
                            r2 = unlink(buf);
                        }
                    }
                    free(buf);
                }

                r = r2;
            }

            closedir(d);
        }

        if (!r) {
            r = rmdir(path);
        }

        return r;
    }
};

class SystemUtility {
public:
    static std::string Exec(const std::string& cmd) {
        return execSystemCommand(cmd.c_str());
    }

    static std::string Diff(const std::string& before_file_path, const std::string& after_file_path) {
        return Exec(std::string("git diff ") + before_file_path + " " + after_file_path);
    }

private:
    static std::string execSystemCommand(const char* cmd) {
        std::array<char, 128> buffer;
        std::string result;
        std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);

        if (!pipe) {
            throw std::runtime_error("popen() failed!");
        }

        while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
            result += buffer.data();
        }

        return result;
    }
};

class Snapshot {
    friend class SnapshotTest;

public:
    template <typename T>
    static void GenerateSnapshot(const T& t, const char* file_name, const char* func_name, const int line_number,
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
        auto& line_number_vec = getLineNumberVec(file_name, file_content.size());
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
    static void GenerateSnapshotDiff(const T& before, const T& after, const char* file_name, const char* func_name,
            const int line_number, const std::vector<std::string>& custom_keys = std::vector<std::string>({})) {
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
    static std::pair<int, int> getSnapshotInlineMatchRange(const std::string& s) {
        const std::string prefix = "SNAPSHOT_INLINE(";

        int len = s.length();
        int prefix_len = prefix.length();

        for (int i = prefix.length() - 1; i + 1 < len; i++) {
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

    static std::string replaceSnapshotInlineContent(
            std::pair<int, int> replace_range, const std::string& origin_s, const std::string& replace_s) {
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

    static std::string getSnapshotKey(const char* file_name, const char* func_name, const int line_number,
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
