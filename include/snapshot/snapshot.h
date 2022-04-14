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

    template <typename T>
    static std::string ToString(const std::vector<T>& v) {
        if (v.empty()) {
            return "{}";
        }

        std::vector<std::string> string_vec = getStringVector(v);
        char delimiter = ',';

        std::string res = "{";
        res += string_vec[0];

        for (size_t i = 1; i < string_vec.size(); i++) {
            res += delimiter;
            res += string_vec[i];
        }

        res += "}";

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
    static char getDelimiter(const std::vector<std::string>& string_vec) {
        char delimiter = ' ';
        for (const auto& s : string_vec) {
            if (s.find('\n') != std::string::npos) {
                delimiter = '\n';
                break;
            }
        }

        return delimiter;
    }

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

    static std::unique_ptr<std::ofstream> OpenFileWithAppendWrite(const std::string& path) {
        std::unique_ptr<std::ofstream> out_file_ptr = std::make_unique<std::ofstream>();
        out_file_ptr->open(path, std::ofstream::out | std::ofstream::ate | std::ofstream::app);
        return out_file_ptr;
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
public:
    template <typename T>
    static void GenerateSnapshot(const T& t, const char* file_name, const char* func_name, const int line_number) {
        const auto content = StringUtility::ToString(t);
        const auto filename_split = StringUtility::Split(file_name, '/');
        const auto snapshot_key = getSnapshotKey(file_name, func_name, line_number);
        const auto snapshot_filename = getSnapshotFilename(filename_split.back().c_str());
        const auto snapshot_dir = getSnapshotDir(filename_split);
        const auto snapshot_target_file = StringUtility::Join({snapshot_dir, snapshot_filename}, '/');

        if (isFirstEnter(snapshot_dir)) {
            FileUtility::RemoveDirectory(snapshot_dir);
        }

        if (!FileUtility::Mkdir(snapshot_dir)) {
            return;
        }

        if (isFirstEnter(file_name)) {
            FileUtility::RemoveFile(snapshot_target_file);
        }

        auto out_file_ptr = FileUtility::OpenFileWithAppendWrite(snapshot_target_file);
        if (!out_file_ptr) {
            return;
        }

        out_file_ptr->write("// ", 3);
        out_file_ptr->write(snapshot_key.c_str(), snapshot_key.length());
        out_file_ptr->write("\n", 1);
        out_file_ptr->write(content.c_str(), content.length());
        out_file_ptr->write("\n", 1);
    }

    template <typename T>
    static void GenerateSnapshotInline(
            const T& t, const char* file_name, const char* func_name, const int line_number) {}

protected:
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

    static std::string getSnapshotFilename(const char* file_name) {
        return std::string(file_name) + std::string(".snap");
    }

    static std::string getSnapshotKey(const char* file_name, const char* func_name, const int line_number) {
        std::string key = "";

        key += file_name;
        key += ".";
        key += func_name;
        key += ".";
        key += StringUtility::ToString(line_number);

        const int ix = count[key]++;

        key += ".";
        key += StringUtility::ToString(ix);

        return key;
    }

    static bool isFirstEnter(const std::string& key) {
        if (count[key]++ == 0) {
            return true;
        }

        return false;
    }

    inline static std::map<std::string, int> count;
};

}  // namespace snapshot

#define SNAPSHOT(content) snapshot::Snapshot::GenerateSnapshot(content, __FILE__, __FUNCTION__, __LINE__)
#define SNAPSHOT_INLINE(content) snapshot::Snapshot::GenerateSnapshotInline(content, __FILE__, __FUNCTION__, __LINE__)

#endif  // SNAPSHOT_SNAPSHOT_H
