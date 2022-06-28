#ifndef SNAPSHOT_INTERNAL_UTILITY_H_
#define SNAPSHOT_INTERNAL_UTILITY_H_

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

}  // namespace snapshot

#endif  // SNAPSHOT_INTERNAL_UTILITY_H_
