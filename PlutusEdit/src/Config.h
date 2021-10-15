#pragma once
#include <vector>
#include <string>


namespace Plutus
{
    struct AppConfig
    {
        int winWidth = 1280;
        int winHeight = 768;
        std::vector<std::string> recentProjects;


        ~AppConfig();
        static AppConfig& get();

    private:
        AppConfig() = default;
        void save();
        void load();

    };

    extern AppConfig& Config;
} // namespace Plutus