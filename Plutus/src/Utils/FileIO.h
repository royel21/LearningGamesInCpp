#pragma once
#include <vector>
#include <string>
#include <sstream>

#ifdef _WIN32
#define sep '\\'
#else
#define sep '/'
#endif

namespace Plutus
{
    namespace FileIO
    {
        void mkdirs(std::string path);

        std::vector<unsigned char> readFile(const char* path2File, const char* mode = "rb");

        std::string readFileAsString(const char* path);

        void saveBufferToFile(const std::string& path, const char* buffer);

        bool exists(const std::string& path);

        bool copyFile(const std::string& src, const std::string dst);

        bool moveFile(const std::string& oldFile, const std::string& newFile);

        void openDir(const std::string& dir);

        template <typename... TArgs>
        std::string joinPath(TArgs ...args) {
            std::stringstream ss;
            int i = 0;
            auto size = sizeof...(args) - 1;
            ([&](const auto& arg) {
                if (i == 0) {
                    ss << arg;
                }
                else {
                    auto str = ss.str();
                    if (str[str.length() - 1] == sep) {
                        ss << arg;
                    }
                    else {
                        ss << sep << arg;
                    }
                }
                i++;
                }(args), ...);
            return ss.str();
        }
    } // namespace FileIO



} // namespace Plutus