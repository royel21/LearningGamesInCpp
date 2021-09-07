#include "Utils.h"
#include <fstream>
#include <cmath>
#include <filesystem>

#define PI 3.141592653589793238463
#define DEC2RA(dec) dec *(PI / 180)

constexpr char separator = std::filesystem::path::preferred_separator;

namespace Plutus
{
    namespace Utils
    {
        std::string getFileName(const std::string& filePath)
        {
            auto index = filePath.find_last_of(separator);
            std::cout << index << std::endl;
            if (index < -1)
            {
                return filePath.substr(index + 1, filePath.size());
            }
            else
            {
                return std::string("no found");
            }
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
            return it != vec.end() ? it - vec.begin() : -1;
        }
    } // namespace Utils
} // namespace Plutus
