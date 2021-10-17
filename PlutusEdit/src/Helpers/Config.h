#pragma once
#include <vector>
#include <string>
#include <unordered_map>


namespace Plutus
{
    struct AppConfig
    {
        int winWidth = 1280;
        int winHeight = 768;
        std::string CurrentProject = "";
        std::vector<std::string> Projects = {};
        std::unordered_map<std::string, std::string> project = {};

        void LoadProject(const char* projPath);
        void CreateProj(const char* filePath);

        ~AppConfig();
        static AppConfig& get();

    private:
        AppConfig() = default;
        void save();
        void load();

    };

    extern AppConfig& Config;
} // namespace Plutus