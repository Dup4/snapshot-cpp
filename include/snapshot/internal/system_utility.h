#ifndef SNAPSHOT_INTERNAL_SYSTEM_UTILITY_H_
#define SNAPSHOT_INTERNAL_SYSTEM_UTILITY_H_

#include <array>
#include <memory>
#include <string>

namespace snapshot {

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

}  // namespace snapshot

#endif  // SNAPSHOT_INTERNAL_SYSTEM_UTILITY_H_
