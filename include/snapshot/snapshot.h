#ifndef SNAPSHOT_SNAPSHOT_H
#define SNAPSHOT_SNAPSHOT_H

#include <unistd.h>
#include <array>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

namespace snapshot {

class StringUtility {
public:
    template <typename T>
    static std::string ToString(const T&& t) {
        std::stringstream ss;
        std::string s;
        ss << t;
        ss >> s;
        return s;
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
        return std::string(s);
    }

    static std::string ToString(const std::string& s) {
        return s;
    }

    template <typename T>
    static std::string ToString(const std::vector<T>& v) {
        if (v.empty()) {
            return "";
        }

        std::vector<std::string> string_vec = getStringVector(v);
        char delimiter = getDelimiter(string_vec);

        std::string res = string_vec[0];

        for (size_t i = 1; i < string_vec.size(); i++) {
            res += delimiter;
            res += string_vec[i];
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
    static std::vector<std::string> getStringVector(const std::vector<T>&& vec) {
        std::vector<std::string> res;
        for (const auto&& t : vec) {
            res.push_back(ToString(t));
        }

        return res;
    }
};

class Snapshot {
public:
protected:
    static std::string exec(const char* cmd) {
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

    static std::string getPWD() {
        return exec("pwd");
    }

    static std::string getSnapshotsDirname() {
        return "__snapshots__";
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

    inline static std::map<std::string, int> count;
};

}  // namespace snapshot

#define SNAPSHOT
#define INLINE_SNAPSHOT
#define SNAPSHOT_TO_FILE

#endif  // SNAPSHOT_SNAPSHOT_H
