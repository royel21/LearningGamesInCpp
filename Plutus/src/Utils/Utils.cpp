#include "Utils.h"

#include <fstream>
#include <filesystem>

#define PI 3.141592653589793238463
#define DEC2RA(dec) dec *(PI / 180)

constexpr char separator = std::filesystem::path::preferred_separator;

namespace Plutus
{
    namespace Utils
    {
        std::string getDirectory(const std::string& filePath)
        {
            return std::filesystem::path(filePath).remove_filename().string();
        }

        std::string getFileName(const std::string& filePath)
        {
            return std::filesystem::path(filePath).filename().string();
        }

        std::string getExtension(const std::string& fileName)
        {
            auto pos = fileName.find_last_of(".");
            if (pos != std::string::npos)
            {
                return fileName.substr(pos + 1);
            }
            return "";
        }

        std::vector<std::string> listFiles(const std::string& _path, const std::string& fileType)
        {
            std::vector<std::string> files;
            if (std::filesystem::exists(_path))
            {
                for (const auto& entry : std::filesystem::directory_iterator(_path))
                {
                    auto path = entry.path();
                    if (fileType.compare(path.extension().string()) == 0)
                    {
                        files.push_back({ _path + "/" + path.filename().string() });
                    }
                }
            }
            return files;
        }

        int getIndex(const std::vector<std::string>& vec, const std::string& s)
        {
            auto it = std::find_if(vec.begin(), vec.end(), [s](auto& s1) -> bool
                { return s1.compare(s) == 0; });
            return it != vec.end() ? int(it - vec.begin()) : -1;
        }

        bool createFile(const char* file, const char* content, bool asString)
        {
            if (std::filesystem::exists(file))  return true;

            std::ofstream outfile;
            outfile.open(file, asString ? std::ios_base::out : std::ios_base::binary); //std::ios_base::app
            if (outfile.is_open()) {
                outfile << content;
                outfile.close();
                return true;
            }
            return false;
        }

        void saveFile(const char* filePath, const char* buffer) {
            std::ofstream outfile;
            outfile.open(filePath, std::ios_base::out); //std::ios_base::app
            outfile << buffer;
            outfile.close();
        }

        void replaceAll(std::string& str, char what, char with)
        {
            size_t i = 0;
            while (i++ < str.length()) {
                if (str[i] == what) str[i] = with;
            }
        }

        std::string ToLowerCase(const std::string& str)
        {
            auto str2 = str;
            for (auto& s : str2) if (s < 91) s = s + 32;
            return str2;
        }
    } // namespace Utils
} // namespace Plutus
