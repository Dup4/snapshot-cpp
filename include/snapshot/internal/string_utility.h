#ifndef SNAPSHOT_INTERNAL_STRING_UTILITY_H_
#define SNAPSHOT_INTERNAL_STRING_UTILITY_H_

#include <sstream>
#include <string>
#include <type_traits>
#include <vector>

#include "../types_check/has_class_internal_to_string.h"
#include "../types_check/has_global_to_string.h"
#include "../types_check/has_operator_stream.h"
#include "../types_check/has_std_to_string.h"

namespace snapshot {

class StringUtility {
    friend class StringUtilityTest;

public:
    template <typename T, std::enable_if_t<internal::has_std_to_string_v<T>, bool> = true>
    static std::string ToString(const T& t) {
        return std::to_string(t);
    }

    template <typename T, std::enable_if_t<internal::has_global_to_string_v<T>, bool> = true>
    static std::string ToString(const T& t) {
        return to_string(t);
    }

    template <typename T, std::enable_if_t<internal::has_class_internal_to_string_v<T>, bool> = true>
    static std::string ToString(const T& t) {
        return t.ToString();
    }

    template <typename T, std::enable_if_t<internal::has_operator_stream_v<T>, bool> = true>
    static std::string ToString([[maybe_unused]] const T& t) {
        std::stringstream ss;
        std::string s;
        ss << t;
        ss >> s;
        return ss.str();
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
        int len = static_cast<int>(s.length());

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
            const auto* p = escape_character_set;
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

}  // namespace snapshot

#endif  // SNAPSHOT_INTERNAL_STRING_UTILITY_H_
