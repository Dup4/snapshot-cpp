#ifndef SNAPSHOT_INTERNAL_FILE_UTILITY_H
#define SNAPSHOT_INTERNAL_FILE_UTILITY_H

#include <ctype.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fstream>
#include <string>
#include <vector>

namespace snapshot {

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
            // don't consider creation failed
            if (ret == 0 || errno == EEXIST) {
                return true;
            }

            return false;
        };

        while (true) {
            size_t cur_pos = path.find('/', pos + 1);
            pos = static_cast<int>(cur_pos);

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
        file.write(content.c_str(), static_cast<long>(content.length()));
        file.close();
    }

    static void AppendFile(const std::string& file_path, const std::string& content) {
        std::ofstream file;
        file.open(file_path, std::ofstream::out | std::ofstream::ate | std::ofstream::app);
        file.write(content.c_str(), static_cast<long>(content.length()));
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
            struct dirent* p = nullptr;

            r = 0;
            while (!r && (p = readdir(d))) {
                int r2 = -1;
                char* buf = nullptr;
                size_t len = 0;

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

}  // namespace snapshot

#endif  // SNAPSHOT_INTERNAL_FILE_UTILITY_H
