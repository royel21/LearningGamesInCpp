#include "Utils.h"
#include <fstream>
#include <cmath>
#include <filesystem>
#include "Assets/IOManager.h"

#define PI 3.141592653589793238463
#define DEC2RA(dec) dec *(PI / 180)

namespace Plutus
{
    namespace Utils
    {
        std::string getFileName(const std::string& filePath)
        {
            auto index = filePath.find_last_of('\\');
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

        bool loadJson(const char* filePath, rapidjson::Document* document)
        {
            std::string ex = getExtension(filePath);
            if (ex == "json")
            {
                auto data = IOManager::readFileToString(filePath);
                if (!data.empty())
                {
                    return document->Parse(data.c_str()).HasParseError() == false;
                }
            }
            return false;
        }

        void toJsonFile(const std::string& path, const char* buffer)
        {
            std::ofstream outfile;
            outfile.open(path.c_str(), std::ios_base::out); //std::ios_base::app
            outfile << buffer;
            outfile.close();
        }

        std::vector<std::string> listFiles(const std::string& path, const std::string& fileType)
        {
            std::vector<std::string> files;
            if (std::filesystem::exists(path)) {
                for (const auto& entry : std::filesystem::directory_iterator(path))
                {
                    auto path = entry.path();
                    if (fileType.compare(path.extension().string()) == 0) {
                        files.push_back(path.string());
                    }
                }
            }
            return files;
        }

        int getIndex(const std::vector<std::string>& vec, const std::string& s)
        {
            auto it = std::find_if(vec.begin(), vec.end(), [s](auto& s1)-> bool { return s1.compare(s) == 0; });
            return it != vec.end() ? it - vec.begin() : -1;
        }
    } // namespace Utils
} // namespace Plutus
